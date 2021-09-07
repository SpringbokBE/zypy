/**
 *  @file    main.cc
 *  @author  Gerbrand De Laender
 *  @date    27/01/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 *  DMA loopback configuration to test latency of DMA streams.
 *
 */
#include <stdio.h>
#include <xparameters.h>
#include <xil_printf.h>

#include "platform.h"
#include "xaxidma.h"
#include "xtime_l.h"

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

	// Initialize all IP cores.
	XAxiDma dma;
	XAxiDma_Config dma_cfg;
	init_axidma(&dma, &dma_cfg);

	XTime t1, t2;

	// Populate the test buffers.
	// DMA in configuration 1 is setup for transfers up to 2^14 - 1 bytes.
	int max_buf_size = 2048; // 2^11 integers is the maximum transfer size.
	int * tx_buf = (int *) (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x00100000);
	int * rx_buf = (int *) (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x00300000);

	for (int i = 0; i <= max_buf_size; ++i)
		tx_buf[i] = i;

	// Measure execution time of an empty run.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty loop took %f us (%llu ticks).\n",
			1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	// Measure execution time of AXI DMA transfer.
	printf("\nStarting timing analysis:\n---\n");

	// Flush the data cache before the transfer to prevent reading stale values.
	Xil_DCacheFlushRange((UINTPTR) tx_buf, max_buf_size * sizeof(int));
	Xil_DCacheFlushRange((UINTPTR) rx_buf, max_buf_size * sizeof(int));

	int status = 0;
	for (int buf_size = 2; buf_size <= max_buf_size; buf_size *= 2) {
		XTime_GetTime(&t1);
		status = XAxiDma_SimpleTransfer(&dma, (UINTPTR) tx_buf,
				buf_size * sizeof(int), XAXIDMA_DMA_TO_DEVICE);

		if (status != XST_SUCCESS)
			return XST_FAILURE;

		status = XAxiDma_SimpleTransfer(&dma, (UINTPTR) rx_buf,
				buf_size * sizeof(int), XAXIDMA_DEVICE_TO_DMA);

		if (status != XST_SUCCESS)
			return XST_FAILURE;

		while (XAxiDma_Busy(&dma, XAXIDMA_DMA_TO_DEVICE)
				|| XAxiDma_Busy(&dma, XAXIDMA_DEVICE_TO_DMA))
			;
		XTime_GetTime(&t2);
		printf("Transfer of %d integers took %f us (%llu ticks).\n", buf_size,
				1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));
	}

	cleanup_platform();
	return 0;
}
