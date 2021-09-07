/**
 *  @file    mmul.cc
 *  @author  Gerbrand De Laender
 *  @date    18/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 *  Uses IP that calculates the matrix multiplication AxB=C with variable dimensions up to 64x64.
 *
 */

#include <stdio.h>
#include <xparameters.h>

#include "platform.h"
#include "xmmul_v2.h"
#include "xaxidma.h"
#include "xtime_l.h"

// The cache line lengths for both L1 caches and the L2 cache are 32 bytes.
// See https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
#define CACHE_ALIGNED __attribute__ ((__aligned__(32)))
#define DEBUG

typedef unsigned int T;

void printmat(T *mat, char *name, int rows, int cols, int transpose)
{
  printf ("%s = \n",name);

  if(!transpose) {
	  for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			#ifdef typefloat
				printf ("%5.2f  ", mat[i * cols + j]);
			#elif defined typedouble
				printf ("%5.2lf  ", mat[i * cols + j]);
			#else
				  printf ("%d  ", mat[i * cols + j]);
			#endif
		}
		printf ("\n");
	  }
  } else {
	  for (int i = 0; i < rows; i++) {
	  		for (int j = 0; j < cols; j++) {
	  			#ifdef typefloat
	  				printf ("%5.2f  ", mat[j * rows + i]);
	  			#elif defined typedouble
	  				printf ("%5.2lf  ", mat[j * rows + i]);
	  			#else
	  				  printf ("%d  ", mat[j * rows + i]);
	  			#endif
	  		}
	  		printf ("\n");
	  }
  }
}

void init_mmul_v2(XMmul_v2 *ip, XMmul_v2_Config *ip_cfg)
{
	ip_cfg = XMmul_v2_LookupConfig(XPAR_MMUL_V2_0_DEVICE_ID);

	if(ip_cfg){
		int status = XMmul_v2_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'mmul_v2'!\n");
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

	const int L = 46;
	const int M = 46;
	const int N = 46;
	const int AB_SIZE = L * M + M * N;
	const int C_SIZE = L * N;

	// Initialize IP cores.
	XMmul_v2 m;	XMmul_v2_Config m_cfg;
	init_mmul_v2(&m, &m_cfg);

	XAxiDma dma; XAxiDma_Config dma_cfg;
	init_axidma(&dma, &dma_cfg);

    XTime t1, t2;

    // Initialize structures.
    T a[L][M], b[M][N], c_sw[L][N];
    T CACHE_ALIGNED ab[L * M + M * N];
    T CACHE_ALIGNED c_hw[N][L];

    T *A = (T *)a;
    T *B = (T *)b;
    T *AB = (T *)ab;
    T *C_SW = (T *)c_sw;
    T *C_HW = (T *)c_hw;
    int x = 0;

	for(int i = 0; i < L; ++i)
		for(int j = 0; j < M; ++j, ++x) {
			a[i][j] = i + j + 1;
			ab[x] = i + j + 1;
		}

	for(int j = 0; j < N; ++j)
		for(int i = 0; i < M; ++i, ++x) {
			b[i][j] = (i * j) + 1;
			ab[x] = (i * j) + 1;
		}

	// Start test.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nSoftware implementation:\n---\n");
	XTime_GetTime(&t1);
	for (int i = 0; i < L; ++i)
		for (int j = 0; j < N; ++j) {
			T sum = 0;
			for (int k = 0; k < M; ++k)
				sum += a[i][k] * b[k][j];
			c_sw[i][j] = sum;
		}
	XTime_GetTime(&t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nHardware implementation:\n---\n");
	XTime_GetTime(&t1);

	XMmul_v2_Set_l_V(&m, L);
	XMmul_v2_Set_m_V(&m, M);
	XMmul_v2_Set_n_V(&m, N);
	XMmul_v2_Start(&m);

	// https://stackoverflow.com/a/42263407
	Xil_DCacheFlushRange((u32) AB, AB_SIZE * sizeof(T));

	XAxiDma_SimpleTransfer(&dma, (UINTPTR) AB, AB_SIZE * sizeof(T), XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_SimpleTransfer(&dma, (UINTPTR) C_HW, C_SIZE * sizeof(T), XAXIDMA_DEVICE_TO_DMA);

	while(XAxiDma_Busy(&dma, XAXIDMA_DEVICE_TO_DMA));

	Xil_DCacheInvalidateRange((u32) C_HW, C_SIZE * sizeof(T));

	XTime_GetTime(&t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nValidation:\n---\n");
	int err = 0;
	for (int i = 0; i < L; ++i)
		for (int j = 0; j < N; ++j)
			if (c_sw[i][j] != c_hw[j][i]) {
				printf("Error at (%d, %d): %x != %x\n", i, j, c_sw[i][j], c_hw[j][i]);
				++err;
			}

	if (!err)
		printf("Test succeeded!\n");
	else
		printf("Test failed!\n");

	cleanup_platform();
	return 0;
}
