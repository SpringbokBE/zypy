/**
 *  @file    main_loopback.c
 *  @author  Gerbrand De Laender
 *  @date    29/07/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <xparameters.h>

#include "platform.h"
#include "xaxidma.h"
#include "xtime_l.h"

#include "xaxis_sink.h"

#define TEST_STREAM_LEN (1U << 23)
//#define DOUBLE_DMA  // Separate the MM2S and S2MM channel over different DMAs.

// The cache line lengths for both L1 caches and the L2 cache are 32 bytes.
// See https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
#define CACHE_ALIGNED __attribute__ ((__aligned__(32)))

void init_axis_sink(XAxis_sink *ip, XAxis_sink_Config *ip_cfg)
{
	ip_cfg = XAxis_sink_LookupConfig(XPAR_AXIS_SINK_0_DEVICE_ID);

	if(ip_cfg){
		int status = XAxis_sink_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'XAXis_sink'!\n");
	}
}

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

int main() {

	//
	// Initialisation of platform and required components.
	//

	init_platform();

	XAxis_sink ip;
	XAxis_sink_Config ip_cfg;

	init_axis_sink(&ip, &ip_cfg);

	XAxiDma dma_0, dma_1;
	XAxiDma_Config dma_0_cfg, dma_1_cfg;

	init_axidma(&dma_0, &dma_0_cfg, XPAR_AXIDMA_0_DEVICE_ID);
	init_axidma(&dma_1, &dma_1_cfg, XPAR_AXIDMA_1_DEVICE_ID);

	#if defined DOUBLE_DMA
	XAxiDma dma_2/*, dma_3*/;
	XAxiDma_Config dma_2_cfg/*, dma_3_cfg*/;

	init_axidma(&dma_2, &dma_2_cfg, XPAR_AXIDMA_2_DEVICE_ID);
//	init_axidma(&dma_3, &dma_3_cfg, XPAR_AXIDMA_3_DEVICE_ID);
	#endif

    XTime t1, t2;

    //
    // Initialisation of the test data.
    //

    static int CACHE_ALIGNED in1[TEST_STREAM_LEN], in2[TEST_STREAM_LEN], out[TEST_STREAM_LEN];

	for(int i = 0; i < TEST_STREAM_LEN; i++) {
		in1[i] = i + 1;
		in2[i] = i + 1;
	}

	//
	// Benchmark the hardware implementation of the SQUARE operation.
	//

	printf("\nHardware implementation:\n---\n");
	Xil_DCacheFlushRange((INTPTR) in1, TEST_STREAM_LEN * sizeof(int)); // https://stackoverflow.com/a/42263407
	Xil_DCacheFlushRange((INTPTR) in2, TEST_STREAM_LEN * sizeof(int)); // https://stackoverflow.com/a/42263407
	XAxis_sink_Start(&ip);

	XTime_GetTime(&t1);

	XAxiDma_SimpleTransfer(&dma_0, (UINTPTR) in1, TEST_STREAM_LEN * sizeof(int), XAXIDMA_DMA_TO_DEVICE);

	#if defined DOUBLE_DMA
	XAxiDma_SimpleTransfer(&dma_2, (UINTPTR) out, TEST_STREAM_LEN * sizeof(int), XAXIDMA_DEVICE_TO_DMA);

	while(XAxiDma_Busy(&dma_2, XAXIDMA_DEVICE_TO_DMA));
	#else
	XAxiDma_SimpleTransfer(&dma_0, (UINTPTR) out, TEST_STREAM_LEN * sizeof(int), XAXIDMA_DEVICE_TO_DMA);

	while(XAxiDma_Busy(&dma_0, XAXIDMA_DEVICE_TO_DMA));
	#endif

	XTime_GetTime(&t2);
	Xil_DCacheInvalidateRange((INTPTR) out, TEST_STREAM_LEN * sizeof(int));

	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	cleanup_platform();
	return 0;
}
