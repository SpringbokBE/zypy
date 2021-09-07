/**
 *  @file    main.cc
 *  @author  Gerbrand De Laender
 *  @date    05/05/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xparameters.h>

#include <iostream>

#include "platform.h"
#include "xaxidma.h"
#include "xtime_l.h"

#define DEBUG
//#define SINGLE_IO  // Use 32-bit input/output for IP
//#define DOUBLE_IO  // Use 64-bit input/output for IP
//#define QUAD_IO  // Use 128-bit input/output for IP
#define OCTA_IO  // Use 256-bit input/output for IP
#define DOUBLE_DMA  // Use two DMAs for both input and output

// The cache line lengths for both L1 caches and the L2 cache are 32 bytes.
// See https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
#define CACHE_ALIGNED __attribute__ ((__aligned__(32)))

// Print out an array in a human-readable format.
void print_arr(float *arr, std::string name, int len)

{
	std::cout << name << "\n---\n";
	for (int i = 0; i < len; i++)
			printf("%5.2f  ", arr[i]);
	printf("\n");
}

#if defined SINGLE_IO
	#include "xnp_sin_f4_hw.h"
	void init_sin_f4_hw(XNp_sin_f4_hw *ip, XNp_sin_f4_hw_Config *ip_cfg)
	{
		ip_cfg = XNp_sin_f4_hw_LookupConfig(XPAR_NP_SIN_F4_HW_0_DEVICE_ID);

		if(ip_cfg){
			int status = XNp_sin_f4_hw_CfgInitialize(ip, ip_cfg);

			if (status != XST_SUCCESS)
				printf("Error initializing 'np_sin_f4_hw'!\n");
		}
	}
#elif defined DOUBLE_IO
	#include "xnp_sin_2f4_hw.h"
	void init_sin_2f4_hw(XNp_sin_2f4_hw *ip, XNp_sin_2f4_hw_Config *ip_cfg)
	{
		ip_cfg = XNp_sin_2f4_hw_LookupConfig(XPAR_NP_SIN_2F4_HW_0_DEVICE_ID);

		if(ip_cfg){
			int status = XNp_sin_2f4_hw_CfgInitialize(ip, ip_cfg);

			if (status != XST_SUCCESS)
				printf("Error initializing 'np_sin_2f4_hw'!\n");
		}
	}
#elif defined QUAD_IO
	#include "xnp_sin_4f4_hw.h"
	void init_sin_4f4_hw(XNp_sin_4f4_hw *ip, XNp_sin_4f4_hw_Config *ip_cfg)
	{
		ip_cfg = XNp_sin_4f4_hw_LookupConfig(XPAR_NP_SIN_4F4_HW_0_DEVICE_ID);

		if(ip_cfg){
			int status = XNp_sin_4f4_hw_CfgInitialize(ip, ip_cfg);

			if (status != XST_SUCCESS)
				printf("Error initializing 'np_sin_4f4_hw'!\n");
		}
	}
#elif defined OCTA_IO
	#include "xnp_sin_8f4_hw.h"
	void init_sin_8f4_hw(XNp_sin_8f4_hw *ip, XNp_sin_8f4_hw_Config *ip_cfg)
	{
		ip_cfg = XNp_sin_8f4_hw_LookupConfig(XPAR_NP_SIN_8F4_HW_0_DEVICE_ID);

		if(ip_cfg){
			int status = XNp_sin_8f4_hw_CfgInitialize(ip, ip_cfg);

			if (status != XST_SUCCESS)
				printf("Error initializing 'np_sin_8f4_hw'!\n");
		}
	}
#endif

void init_axidma_0(XAxiDma *ip, XAxiDma_Config *ip_cfg) {
	ip_cfg = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);

	if (ip_cfg) {
		int status = XAxiDma_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'axidma'!\n");
	}

	// Disable interrupts for now.
	XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

void init_axidma_1(XAxiDma *ip, XAxiDma_Config *ip_cfg) {
	ip_cfg = XAxiDma_LookupConfig(XPAR_AXIDMA_1_DEVICE_ID);

	if (ip_cfg) {
		int status = XAxiDma_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'axidma'!\n");
	}

	// Disable interrupts for now.
	XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

#if defined DOUBLE_DMA
	void init_axidma_2(XAxiDma *ip, XAxiDma_Config *ip_cfg) {
		ip_cfg = XAxiDma_LookupConfig(XPAR_AXIDMA_2_DEVICE_ID);

		if (ip_cfg) {
			int status = XAxiDma_CfgInitialize(ip, ip_cfg);

			if (status != XST_SUCCESS)
				printf("Error initializing 'axidma'!\n");
		}

		// Disable interrupts for now.
		XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
		XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	}

	void init_axidma_3(XAxiDma *ip, XAxiDma_Config *ip_cfg) {
		ip_cfg = XAxiDma_LookupConfig(XPAR_AXIDMA_3_DEVICE_ID);

		if (ip_cfg) {
			int status = XAxiDma_CfgInitialize(ip, ip_cfg);

			if (status != XST_SUCCESS)
				printf("Error initializing 'axidma'!\n");
		}

		// Disable interrupts for now.
		XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
		XAxiDma_IntrDisable(ip, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	}
#endif

int main() {
	init_platform();

	// Initialize IP cores.
#if defined SINGLE_IO
	XNp_sin_f4_hw ip; XNp_sin_f4_hw_Config ip_cfg;
	init_sin_f4_hw(&ip, &ip_cfg);
#elif defined DOUBLE_IO
	XNp_sin_2f4_hw ip; XNp_sin_2f4_hw_Config ip_cfg;
	init_sin_2f4_hw(&ip, &ip_cfg);
#elif defined QUAD_IO
	XNp_sin_4f4_hw ip; XNp_sin_4f4_hw_Config ip_cfg;
	init_sin_4f4_hw(&ip, &ip_cfg);
#elif defined OCTA_IO
	XNp_sin_8f4_hw ip; XNp_sin_8f4_hw_Config ip_cfg;
	init_sin_8f4_hw(&ip, &ip_cfg);
#endif

	XAxiDma dma_0, dma_1;
	XAxiDma_Config dma_0_cfg, dma_1_cfg;
	init_axidma_0(&dma_0, &dma_0_cfg);
	init_axidma_1(&dma_1, &dma_1_cfg);

#if defined DOUBLE_DMA
	XAxiDma dma_2, dma_3;
	XAxiDma_Config dma_2_cfg, dma_3_cfg;
	init_axidma_2(&dma_2, &dma_2_cfg);
	init_axidma_3(&dma_3, &dma_3_cfg);
#endif

    XTime t1, t2;

	// Initialize data structures.
	const int LEN = 2 << 19; // Length of stream for testing.
    float CACHE_ALIGNED a[LEN], b_hw[LEN], b_sw[LEN];

	for(int i = 0; i < LEN; i++)
		a[i] = (float) rand() / (float) RAND_MAX;

	// Start test.
	printf("\nSoftware implementation:\n---\n");
	XTime_GetTime(&t1);
	for (int i = 0; i < LEN; i++)
		b_sw[i] = sinf(a[i]);
	XTime_GetTime(&t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nHardware implementation:\n---\n");
	Xil_DCacheFlushRange((INTPTR) a, LEN * sizeof(float)); // https://stackoverflow.com/a/42263407
	XTime_GetTime(&t1);

#if defined SINGLE_IO
	XNp_sin_f4_hw_Start(&ip);
#elif defined DOUBLE_IO
	XNp_sin_2f4_hw_Start(&ip);
#elif defined QUAD_IO
	XNp_sin_4f4_hw_Start(&ip);
#elif defined OCTA_IO
	XNp_sin_8f4_hw_Start(&ip);
#endif

#if defined DOUBLE_DMA
	int first_half = LEN / 2;
	float first_half_n_bytes = first_half * sizeof(float);

	int second_half = LEN - first_half;
	float second_half_n_bytes = second_half * sizeof(float);

	XAxiDma_SimpleTransfer(&dma_0, (UINTPTR) a, first_half_n_bytes, XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(&dma_1, (UINTPTR) (a + first_half), second_half_n_bytes, XAXIDMA_DMA_TO_DEVICE);

	XAxiDma_SimpleTransfer(&dma_2, (UINTPTR) b_hw, first_half_n_bytes, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_SimpleTransfer(&dma_3, (UINTPTR) (b_hw + first_half), second_half_n_bytes, XAXIDMA_DEVICE_TO_DMA);

	while(XAxiDma_Busy(&dma_2, XAXIDMA_DEVICE_TO_DMA) || XAxiDma_Busy(&dma_3, XAXIDMA_DEVICE_TO_DMA));
#else
	XAxiDma_SimpleTransfer(&dma_0, (UINTPTR) a, LEN * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(&dma_1, (UINTPTR) b_hw, LEN * sizeof(float), XAXIDMA_DEVICE_TO_DMA);

	while(XAxiDma_Busy(&dma_1, XAXIDMA_DEVICE_TO_DMA));
#endif

	XTime_GetTime(&t2);
	Xil_DCacheInvalidateRange((INTPTR) b_hw, LEN * sizeof(float));

	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nValidation:\n---\n");
	int err = 0;
	for (int i = 0; i < LEN; i++) {
		float delta = abs(b_sw[i] - b_hw[i]);
		if(delta > 1.0e-07) {
			printf("Error at position '%d': '%.6e' != '%.6e' (delta = %.6e)\n", i, b_sw[i], b_hw[i], delta);
			++err;
		}
	}

	if (!err)
		printf("Test succeeded!\n");
	else
		printf("Test failed!\n");

	cleanup_platform();
	return 0;
}
