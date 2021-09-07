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
 *  Uses IP that applies gain to an incoming AXI stream. Makes use of DMA to
 *  stream data from the main memory to the IP core.
 *
 */

#include <stdio.h>
#include <xparameters.h>
#include <xtime_l.h>

#include "platform.h"
#include "xgain.h"
#include "xaxidma.h"

#define BUFFER_SIZE		1000
#define DMA_BASE_ADDR 	0x0100000 					// Make sure this is out of range of the code!
#define TX_BUF_ADDR 	DMA_BASE_ADDR + 0x00100000 	// Outgoing AXI stream will be placed here.
#define RX_BUF_ADDR 	DMA_BASE_ADDR + 0x00300000 	// Incoming AXI stream will be placed here.

void init_gain(XGain *ip, XGain_Config *ip_cfg) {
	ip_cfg = XGain_LookupConfig(XPAR_XGAIN_0_DEVICE_ID);

	if (ip_cfg) {
		int status = XGain_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'gain'!\n");
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
	XGain g;
	XGain_Config g_cfg;
	init_gain(&g, &g_cfg);

	XAxiDma dma;
	XAxiDma_Config dma_cfg;
	init_axidma(&dma, &dma_cfg);

	XTime t1, t2;

	// Initialize structures.
	int *tx_buffer = reinterpret_cast<int *>(TX_BUF_ADDR);
	int *rx_buffer = reinterpret_cast<int *>(RX_BUF_ADDR);

	for (int i = 0; i < BUFFER_SIZE; i++)
		tx_buffer[i] = i;

	int gain = 5;
	XGain_Set_value_r(&g, gain);
	XGain_Start(&g);

	// Start test.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	// Flush cache of the used buffers.
	Xil_DCacheFlushRange(reinterpret_cast<INTPTR>(tx_buffer),
			BUFFER_SIZE * sizeof(int));
	Xil_DCacheFlushRange(reinterpret_cast<INTPTR>(rx_buffer),
			BUFFER_SIZE * sizeof(int));

	// Send from DMA -> IP
	XTime_GetTime(&t1);
	XAxiDma_SimpleTransfer(&dma, reinterpret_cast<UINTPTR>(tx_buffer),
			BUFFER_SIZE * sizeof(int), XAXIDMA_DMA_TO_DEVICE);

	// Receive from IP -> DMA (blocking).
	XAxiDma_SimpleTransfer(&dma, reinterpret_cast<UINTPTR>(rx_buffer),
			BUFFER_SIZE * sizeof(int), XAXIDMA_DEVICE_TO_DMA);
	while (XAxiDma_Busy(&dma, XAXIDMA_DEVICE_TO_DMA))
		;
	XTime_GetTime(&t2);

	// Flush cache of the used buffers.
	Xil_DCacheFlushRange(reinterpret_cast<INTPTR>(rx_buffer),
			BUFFER_SIZE * sizeof(int));


	printf("Calculation complete!\n");
	for (int i = 0; i < BUFFER_SIZE; i++)
		printf("gain(%d, %d) = %d\n", i, gain, rx_buffer[i]);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	cleanup_platform();
	return 0;
}
