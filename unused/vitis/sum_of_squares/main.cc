/**
 *  @file    main.cc
 *  @author  Gerbrand De Laender
 *  @date    18/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 *  Uses IP that calculates the sum of the squares of the incoming AXI stream.
 *
 */

#include <stdio.h>
#include <xparameters.h>

#include "platform.h"
#include "xsum_of_squares.h"
#include "xaxidma.h"
#include "xtime_l.h"

#define BUFFER_SIZE		1024
#define DMA_BASE_ADDR 	0x0100000 					// Make sure this is out of range of the code!
#define TX_BUF_ADDR 	DMA_BASE_ADDR + 0x00100000 	// Outgoing AXI stream will be placed here.

void init_sum_of_squares(XSum_of_squares *ip, XSum_of_squares_Config *ip_cfg){
	ip_cfg = XSum_of_squares_LookupConfig(XPAR_SUM_OF_SQUARES_0_DEVICE_ID);

	if(ip_cfg){
		int status = XSum_of_squares_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'sum_of_squares'!\n");
	}
}


void init_axidma(XAxiDma *ip, XAxiDma_Config *ip_cfg) {
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

int main() {
	init_platform();

	// Initialize IP cores.
	XSum_of_squares s;
	XSum_of_squares_Config s_cfg;
	init_sum_of_squares(&s, &s_cfg);

	XAxiDma dma;
	XAxiDma_Config dma_cfg;
	init_axidma(&dma, &dma_cfg);

    XTime t1, t2;

    // Initialize structures.
	int A[BUFFER_SIZE];
	int *tx_buffer = reinterpret_cast<int *>(TX_BUF_ADDR);

	for(int i = 0; i < BUFFER_SIZE; i++){
		A[i] = i;
		tx_buffer[i] = i;
	}

	// Start test.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	int res;

	printf("\nSoftware implementation:\n---\n");
	res = 0;
	XTime_GetTime(&t1);
	for(int i = 0; i < BUFFER_SIZE; i++)
		res += (A[i] * A[i]);
	XTime_GetTime(&t2);
	printf("res = %d\n", res);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nHardware implementation:\n---\n");
	XTime_GetTime(&t1);
	XSum_of_squares_Start(&s);

	// Send from DMA -> IP
	XAxiDma_SimpleTransfer(&dma, reinterpret_cast<UINTPTR>(tx_buffer), BUFFER_SIZE * sizeof(int), XAXIDMA_DMA_TO_DEVICE);
	while(!XSum_of_squares_IsDone(&s));
	res = XSum_of_squares_Get_return(&s);
	XTime_GetTime(&t2);
	printf("res = %d\n", res);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	cleanup_platform();
	return 0;
}
