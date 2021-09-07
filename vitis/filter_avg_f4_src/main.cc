#include <stdio.h>
#include <iostream>
#include <math.h>
#include <xparameters.h>

#include "platform.h"
#include "xtime_l.h"

#include "xaxidma.h"
#include "xfilter_avg_f4.h"

///////////////////////////////////////////////////////////////////////////////

#define ERROR_TOLERANCE 0.01
#define TEST_STREAM_LEN (1U << 19)

// The cache line lengths for both L1 caches and the L2 cache are 32 bytes.
// See https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
#define CACHE_ALIGNED __attribute__ ((__aligned__(32)))

///////////////////////////////////////////////////////////////////////////////

// Initialisation of the IP core.

void init_filter_avg_f4(XFilter_avg_f4 *ip, XFilter_avg_f4_Config *ip_cfg)
{
	ip_cfg = XFilter_avg_f4_LookupConfig(XPAR_XFILTER_AVG_F4_0_DEVICE_ID);

	if(ip_cfg){
		int status = XFilter_avg_f4_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'XFilter_avg_f4'!\n");
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

int test_avg_filter(XFilter_avg_f4 *ip, XAxiDma *dma_0, XTime *t1, XTime *t2)
{
	std::cout << "Starting 'avg_filter' test...\n";

	u32 h = (u32) sqrt(TEST_STREAM_LEN);
	u32 w = h;

	CACHE_ALIGNED float in_a[h][w],
						out_a[h][w], out_a_ref[h][w];
    u32 args = (h << 0) | (w << 16);

	// Fill in 'in_a'.

	for(u32 y = 0; y < h; y++)
		for(u32 x = 0; x < w; x++)
			in_a[y][x] = (float) rand() / (float) RAND_MAX;

	// Run reference implementation.

	std::cout << "Running reference implementation...\n";
	XTime_GetTime(t1);

	for(u32 y = 0; y < h; y++)
		for(u32 x = 0; x < w; x++) {
			if(y == 0 or y == h - 1 or x == 0 or x == w - 1)
				out_a_ref[y][x] = in_a[y][x];
			else
				out_a_ref[y][x] = 0.2f * (in_a[y - 1][x] + in_a[y][x - 1] + in_a[y][x] + in_a[y][x + 1] + in_a[y + 1][x]);
		}

	XTime_GetTime(t2);
    printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Run hardware implementation.

    std::cout << "Running hardware implementation...\n";
    Xil_DCacheFlushRange((INTPTR) in_a, h * w * sizeof(float)); // https://stackoverflow.com/a/42263407
    Xil_DCacheInvalidateRange((INTPTR) out_a, h * w * sizeof(float)); // https://stackoverflow.com/a/42263407
	XTime_GetTime(t1);
	XFilter_avg_f4_Set_args(ip, args);
	XFilter_avg_f4_Start(ip);

	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) in_a, h * w * sizeof(float), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(dma_0, (UINTPTR) out_a, h * w * sizeof(float), XAXIDMA_DEVICE_TO_DMA);
	while(XAxiDma_Busy(dma_0, XAXIDMA_DEVICE_TO_DMA));

	XTime_GetTime(t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (*t2 - *t1) / COUNTS_PER_SECOND, 2 * (*t2 - *t1));

	// Check if reference and hardware implementation match.

	for(u32 y = 0; y < h; y++)
		for(u32 x = 0; x < w; x++)
			if(abs(out_a_ref[y][x] - out_a[y][x]) > ERROR_TOLERANCE) {
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

	XFilter_avg_f4 ip;
	XFilter_avg_f4_Config ip_cfg;

	init_filter_avg_f4(&ip, &ip_cfg);

	XAxiDma dma_0;
	XAxiDma_Config dma_0_cfg;

	init_axidma(&dma_0, &dma_0_cfg, XPAR_AXIDMA_0_DEVICE_ID);

    XTime t1, t2;

	bool error = false;
	error |= test_avg_filter(&ip, &dma_0, &t1, &t2);

	cleanup_platform();
	return error;
}

///////////////////////////////////////////////////////////////////////////////
