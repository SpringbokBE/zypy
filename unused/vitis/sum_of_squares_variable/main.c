/**
 *  @file    main.c
 *  @author  Gerbrand De Laender
 *  @date    24/02/2021
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
#include <inttypes.h>

#include "platform.h"
#include "xsum_of_squares_variable.h"
#include "xaxidma.h"
#include "xgpio.h"
#include "xtime_l.h"

#define N 1100

typedef u32 T;

T * tx_buf = (T *)(XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x01000000);

void init_sum_of_squares_variable(XSum_of_squares_variable *ip, XSum_of_squares_variable_Config *ip_cfg)
{
	ip_cfg = XSum_of_squares_variable_LookupConfig(XPAR_SUM_OF_SQUARES_VARIA_0_DEVICE_ID);

	if(ip_cfg){
		int status = XSum_of_squares_variable_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'sum_of_squares_variable'!\n");
	}
}

void init_axidma(XAxiDma *ip, XAxiDma_Config *ip_cfg)
{
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

void init_gpio(XGpio *ip, XGpio_Config *ip_cfg)
{
	ip_cfg = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID);

	if (ip_cfg) {
		int status = XGpio_CfgInitialize(ip, ip_cfg, ip_cfg->BaseAddress);

		if (status != XST_SUCCESS)
			printf("Error initializing 'gpio'!\n");
	}

	XGpio_DiscreteWrite(ip, 1, 1); // Active low reset
}

int main() {
	init_platform();

	// Initialize IP cores.
	XSum_of_squares_variable s;
	XSum_of_squares_variable_Config s_cfg;
	init_sum_of_squares_variable(&s, &s_cfg);

	XAxiDma dma;
	XAxiDma_Config dma_cfg;
	init_axidma(&dma, &dma_cfg);

	XGpio gpio;
	XGpio_Config gpio_cfg;
	init_gpio(&gpio, &gpio_cfg);

    XTime t1, t2;

    // Initialize structures.
	for(int i = 0; i < N; i++)
		tx_buf[i] = (T) i;

	// Measure empty run.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	// Measure execution time in software.
	printf("\nSoftware implementation:\n---\n");
	T res = 0;
	XTime_GetTime(&t1);
	for(int i = 0; i < N; i++)
		res += (tx_buf[i] * tx_buf[i]);
	XTime_GetTime(&t2);
	printf("res = %" PRIu32 "\n", res);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	// Measure execution time in hardware.
	printf("\nHardware implementation:\n---\n");
	XTime_GetTime(&t1);

	const int stream_len = 256;
	int iterations = N / stream_len;
	int remainder = N % stream_len;

	Xil_DCacheFlushRange((u32) tx_buf, N * sizeof(T));

	if (iterations) {
		XSum_of_squares_variable_EnableAutoRestart(&s);
		XSum_of_squares_variable_Set_stream_len_min_one_V(&s, stream_len - 1);
		XSum_of_squares_variable_Start(&s);
		XAxiDma_SimpleTransfer(&dma, (u32) tx_buf, iterations * stream_len * sizeof(T), XAXIDMA_DMA_TO_DEVICE);
	}

	while(XAxiDma_Busy(&dma, XAXIDMA_DMA_TO_DEVICE) || !XSum_of_squares_variable_IsDone(&s));
	res = XSum_of_squares_variable_Get_return(&s);

	if (remainder) {
		if (iterations) {
			XGpio_DiscreteWrite(&gpio, 1, 0);
			XGpio_DiscreteWrite(&gpio, 1, 1);
		}
		XSum_of_squares_variable_Set_stream_len_min_one_V(&s, remainder - 1);
		XSum_of_squares_variable_Start(&s);
		XAxiDma_SimpleTransfer(&dma, (u32)(tx_buf + N - remainder), remainder * sizeof(T), XAXIDMA_DMA_TO_DEVICE);
		while(!XSum_of_squares_variable_IsDone(&s));
		res += XSum_of_squares_variable_Get_return(&s);
	}

	XTime_GetTime(&t2);
	printf("res = %" PRIu32 "\n", res);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	T n = (T) N;
	T check = (n - 1) * n * (2 * (n - 1) + 1) / 6;
	printf("check = %" PRIu32 "\n", check);

	cleanup_platform();
	return 0;
}
