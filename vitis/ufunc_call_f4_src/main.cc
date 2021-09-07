#include <stdio.h>
#include <iostream>
#include <cmath>
#include <xparameters.h>

#include "platform.h"
#include "xtime_l.h"

#include "xaxidma.h"
#include "xufunc_call_f4.h"

///////////////////////////////////////////////////////////////////////////////

// Definition of all supported operations.

#define NONE 0
#define ADD 1
#define MULTIPLY 2
#define SQUARE 3
#define EXP 4
#define LOG 5
#define ABSDIFF 6
#define SUBTRACT 7
#define TRUE_DIVIDE 8
#define SIN 9
#define SQRT 10
#define ARCTAN 11
#define SINH 12

#define ERROR_TOLERANCE 0.01
#define TEST_STREAM_LEN (1U << 21)

// The cache line lengths for both L1 caches and the L2 cache are 32 bytes.
// See https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
#define CACHE_ALIGNED __attribute__ ((__aligned__(32)))

///////////////////////////////////////////////////////////////////////////////

void init_ufunc_call_f4(XUfunc_call_f4 *ip, XUfunc_call_f4_Config *ip_cfg)
{
	ip_cfg = XUfunc_call_f4_LookupConfig(XPAR_XUFUNC_CALL_F4_0_DEVICE_ID);

	if(ip_cfg){
		int status = XUfunc_call_f4_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'XUfunc_call_f4'!\n");
	}
}

///////////////////////////////////////////////////////////////////////////////

// Initialisation of the DMAs.

void init_axi_dma(XAxiDma *ip, XAxiDma_Config *ip_cfg, u32 device_id)
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

int test_add_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'add_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN], in2_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
		in2_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = in1_a[i] + in2_a[i];
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheFlushRange((INTPTR) in2_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, ADD);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_1, (UINTPTR) in2_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_subtract_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'subtract_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN], in2_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
		in2_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = in1_a[i] - in2_a[i];
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheFlushRange((INTPTR) in2_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, SUBTRACT);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_1, (UINTPTR) in2_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_multiply_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'multiply_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN], in2_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
		in2_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = in1_a[i] * in2_a[i];
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheFlushRange((INTPTR) in2_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, MULTIPLY);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_1, (UINTPTR) in2_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_true_divide_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XAxiDma *dma_1, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'true_divide_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN], in2_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
		in2_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = in1_a[i] / in2_a[i];
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheFlushRange((INTPTR) in2_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, TRUE_DIVIDE);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_1, (UINTPTR) in2_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_sin_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'sin_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = sinf(in1_a[i]);
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, SIN);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_sqrt_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'sqrt_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = sqrtf(in1_a[i]);
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, SQRT);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_arctan_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'arctan_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = atanf(in1_a[i]);
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, ARCTAN);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_sinh_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'sinh_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = sinhf(in1_a[i]);
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, SINH);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_exp_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'exp_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = expf(in1_a[i]);
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, EXP);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int test_log_call_f4(XUfunc_call_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'sinh_call' test...\n";

	static CACHE_ALIGNED float in1_a[TEST_STREAM_LEN],
							   out_a[TEST_STREAM_LEN], out_a_ref[TEST_STREAM_LEN];

	// Fill in 'in1_a' and 'in2_a'.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
		in1_a[i] = (float) rand() / (float) RAND_MAX;
	}

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 i = 0; i < TEST_STREAM_LEN; i++) {
			out_a_ref[i] = logf(in1_a[i]);
	}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in1_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, TEST_STREAM_LEN * sizeof(float)); // https://stackoverflow.com/a/42263407

    XTime_GetTime(t1);
	XUfunc_call_f4_Set_args(ip, LOG);
	XUfunc_call_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in1_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, TEST_STREAM_LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 i = 0; i < TEST_STREAM_LEN; i++)
		if(abs(out_a_ref[i] - out_a[i]) > ERROR_TOLERANCE) {
			std::cout << "Test failed!\n";
			printf("out_a_ref = %0.2f <> out_a = %0.2f\n", out_a_ref[i], out_a[i]);
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

	XUfunc_call_f4 ufunc_call_f4;
	XUfunc_call_f4_Config ufunc_call_f4_cfg;

	init_ufunc_call_f4(&ufunc_call_f4, &ufunc_call_f4_cfg);

	XAxiDma dma_0, dma_1;
	XAxiDma_Config dma_0_cfg, dma_1_cfg;

	init_axi_dma(&dma_0, &dma_0_cfg, XPAR_AXIDMA_0_DEVICE_ID);
	init_axi_dma(&dma_1, &dma_1_cfg, XPAR_AXIDMA_1_DEVICE_ID);

    XTime t1, t2;

	bool error = false;
	error |= test_add_call_f4(&ufunc_call_f4, &dma_0, &dma_1, &t1, &t2);
	error |= test_subtract_call_f4(&ufunc_call_f4, &dma_0, &dma_1, &t1, &t2);
	error |= test_multiply_call_f4(&ufunc_call_f4, &dma_0, &dma_1, &t1, &t2);
	error |= test_true_divide_call_f4(&ufunc_call_f4, &dma_0, &dma_1, &t1, &t2);
	error |= test_sin_call_f4(&ufunc_call_f4, &dma_0, &t1, &t2);
	error |= test_sqrt_call_f4(&ufunc_call_f4, &dma_0, &t1, &t2);
	error |= test_arctan_call_f4(&ufunc_call_f4, &dma_0, &t1, &t2);
	error |= test_sinh_call_f4(&ufunc_call_f4, &dma_0, &t1, &t2);
	error |= test_exp_call_f4(&ufunc_call_f4, &dma_0, &t1, &t2);
	error |= test_log_call_f4(&ufunc_call_f4, &dma_0, &t1, &t2);

	cleanup_platform();
	return error;
}

///////////////////////////////////////////////////////////////////////////////
