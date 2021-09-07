#include <stdio.h>
#include <iostream>
#include <cmath>
#include <xparameters.h>

#include "platform.h"
#include "xtime_l.h"

#include "xaxidma.h"
#include "xufunc_reduce_all_f4.h"

///////////////////////////////////////////////////////////////////////////////

// Definition of all supported operations.

#define NONE 0
#define ADD 1
#define MULTIPLY 2
#define SQUARE 3
#define EXP 4
#define LOG 5
#define ABSDIFF 6

#define ERROR_TOLERANCE 0.01
#define TEST_STREAM_LEN (1U << 21)

// The cache line lengths for both L1 caches and the L2 cache are 32 bytes.
// See https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
#define CACHE_ALIGNED __attribute__ ((__aligned__(32)))

///////////////////////////////////////////////////////////////////////////////

// Conversion from unsigned int to float.

float u32_to_float(u32 val) {
	union {
		u32 u4;
		float f4;
	} data = {val};

	return data.f4;
}

///////////////////////////////////////////////////////////////////////////////

// Initialisation of the IP core.

void init_ufunc_reduce_all_f4(XUfunc_reduce_all_f4 *ip, XUfunc_reduce_all_f4_Config *ip_cfg)
{
	ip_cfg = XUfunc_reduce_all_f4_LookupConfig(XPAR_UFUNC_REDUCE_ALL_F4_0_DEVICE_ID);

	if(ip_cfg){
		int status = XUfunc_reduce_all_f4_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'XUfunc_reduce_all_f4'!\n");
	}
}

///////////////////////////////////////////////////////////////////////////////

// Initialisation of the DMAs.

void init_axidma(XAxiDma *ip, XAxiDma_Config *ip_cfg, u32 device_id)
{
	ip_cfg = XAxiDma_LookupConfig(device_id);

	if (ip_cfg) {
		int status = XAxiDma_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'XAxiDma'!\n");
	}

	// Disable interrupts for now.
	XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

///////////////////////////////////////////////////////////////////////////////

int test_add_reduce_all(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'add_reduce_all' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (NONE << 8) | (NONE << 16);

	// Fill in 'in1_s' and 'in1_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		in1_a[i] = 5.0f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = in1_a[0];

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref += in1_a[i];

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// The subtract reduction can be derived from the add reduction using:
// a - b - c - d - ... = a - (b + c + d + ...)

int test_subtract_reduce_all(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'subtract_reduce_all' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (NONE << 8) | (NONE << 16);

	// Fill in 'in1_s' and 'in1_a'.
	in1_a[0] = 5.0f;

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		in1_a[i] = 5.0f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = in1_a[0];

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref -= in1_a[i];

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) (in1_a + 1), (TEST_STREAM_LEN - 1) * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));
	
    out = in1_a[0] - u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_multiply_reduce_all(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'multiply_reduce_all' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (MULTIPLY << 0) | (NONE << 8) | (NONE << 16);

	// Fill in 'in1_s' and 'in1_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
        in1_a[i] = 1.00123f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = in1_a[0];

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref *= in1_a[i];

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// The true_divide reduction can be derived from the multiply reduction using:
// a / b / c / d / ... = a / (b * c * d * ...)

int test_true_divide_reduce_all(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'true_divide_reduce_all' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (MULTIPLY << 0) | (NONE << 8) | (NONE << 16);

	// Fill in 'in1_s' and 'in1_a'.
	in1_a[0] = 1.00123f;

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		in1_a[i] = 1.00123f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = in1_a[0];

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref /= in1_a[i];

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) (in1_a + 1), (TEST_STREAM_LEN - 1) * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = in1_a[0] / u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// The logaddexp reduction can be derived from the sum of exponentials using:
// log(exp(log(exp(a) + exp(b)) + exp(c)) ... = log(exp(a) + exp(b) + exp(c) + ...)

int test_logaddexp_reduce_all(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'logaddexp_reduce_all' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (EXP << 8) | (LOG << 16);

	// Fill in 'in1_s' and 'in1_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		in1_a[i] = 1.00123f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = expf(in1_a[0]);

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref += expf(in1_a[i]);

	out_ref = logf(out_ref);

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// The logaddexp2 reduction can be derived from the sum of exponentials using:
// log2(exp(log(exp(a) + exp(b)) + exp(c)) ... = log2(exp(a) + exp(b) + exp(c) + ...)

int test_logaddexp2_reduce_all(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'logaddexp2_reduce_all' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (EXP << 8) | (LOG << 16);

	// Fill in 'in1_s' and 'in1_a'.
	in1_a[0] = 1.00123f;

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		in1_a[i] = 1.00123f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = expf(in1_a[0]);

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref += expf(in1_a[i]);

	out_ref = log2f(out_ref);

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip)) / logf(2.0f);

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_dot(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'dot' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN], in2_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (MULTIPLY << 8) | (NONE << 16);

	// Fill in 'in1_s'/'in2_s' and 'in1_a'/'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		in1_a[i] = in2_a[i] = 1.00123f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = in1_a[0];

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref += (in1_a[i] * in2_a[i]);

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	Xil_DCacheFlushRange((INTPTR) in2_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_1, (UINTPTR) in2_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_sum_of_squares(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'sum_of_squares' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (SQUARE << 8) | (NONE << 16);

	// Fill in 'in1_s' and 'in1_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		in1_a[i] = 5.0f;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = in1_a[0] * in1_a[0];

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref += in1_a[i] * in1_a[i];

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_sad(XUfunc_reduce_all_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'sad' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN], in2_a[TEST_STREAM_LEN];
    float out, out_ref;
    u32 args = (ADD << 0) | (ABSDIFF << 8) | (NONE << 16);

	// Fill in 'in1_s'/'in2_s' and 'in1_a'/'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++){
		in1_a[i] = (float) i;
		in2_a[i] = (float) (2 * i);
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	out_ref = abs(in1_a[0] - in2_a[0]);

	for(u32 i = 1; i < TEST_STREAM_LEN; i++)
		out_ref += abs(in1_a[i] - in2_a[i]);

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	Xil_DCacheFlushRange((INTPTR) in2_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XUfunc_reduce_all_f4_Set_args(ip, args);
	XUfunc_reduce_all_f4_Start(ip);
	XTime_GetTime(t1);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_1, (UINTPTR) in2_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	while(not XUfunc_reduce_all_f4_IsDone(ip));

	out = u32_to_float(XUfunc_reduce_all_f4_Get_return(ip));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Print out the results.

	printf("out = %.3f\n", out);
	printf("out_ref = %.3f\n", out_ref);

	// Check if reference and hardware implementation match.

	if(abs(out - out_ref) > ERROR_TOLERANCE) {
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
    //
	// Initialisation of platform and required components.
	//

	init_platform();

	XUfunc_reduce_all_f4 ip;
	XUfunc_reduce_all_f4_Config ip_cfg;

	init_ufunc_reduce_all_f4(&ip, &ip_cfg);

	XAxiDma dma_0, dma_1;
	XAxiDma_Config dma_0_cfg, dma_1_cfg;

	init_axidma(&dma_0, &dma_0_cfg, XPAR_AXIDMA_0_DEVICE_ID);
	init_axidma(&dma_1, &dma_1_cfg, XPAR_AXIDMA_1_DEVICE_ID);

    XTime t1, t2;

	bool error = false;
	error |= test_add_reduce_all(&ip, &dma_0, &t1, &t2);
	error |= test_subtract_reduce_all(&ip, &dma_0, &t1, &t2);
	error |= test_multiply_reduce_all(&ip, &dma_0, &t1, &t2);
	error |= test_true_divide_reduce_all(&ip, &dma_0, &t1, &t2);
	error |= test_logaddexp_reduce_all(&ip, &dma_0, &t1, &t2);
	error |= test_logaddexp2_reduce_all(&ip, &dma_0, &t1, &t2);
	error |= test_dot(&ip, &dma_0, &dma_1, &t1, &t2);
	error |= test_sum_of_squares(&ip, &dma_0, &dma_1, &t1, &t2);
	error |= test_sad(&ip, &dma_0, &dma_1, &t1, &t2);

	cleanup_platform();
	return error;
}

///////////////////////////////////////////////////////////////////////////////
