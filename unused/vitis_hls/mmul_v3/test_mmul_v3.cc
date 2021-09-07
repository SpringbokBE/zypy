/**
 *  @file    test_mmul_v3.cc
 *  @author  Gerbrand De Laender
 *  @date    11/03/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'mmul_v3.cc'.
 *
 */

#include <stdio.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "stream.h"

#define MAX_DIM 64
#define L 24
#define M 25
#define N 26

typedef unsigned int T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void mmul_v3(hls::stream<T_SIDE> &AB, hls::stream<T_SIDE> &C, ap_uint<6> l, ap_uint<6> m, ap_uint<6> n);

// Helper function to print out a matrix.
void printmat(T *mat, char *name, int rows, int cols)
{
  printf ("%s = \n",name);
  for (int i = 0; i < rows; i++) {
	for (int j = 0; j < cols; j++) {
		#ifdef typefloat
			printf ("%5.2f  ", mat[i * sizeof(T) + j]);
		#elif defined typedouble
			printf ("%5.2lf  ", mat[i * sizeof(T) + j]);
		#else
			  printf ("%d  ", mat[i * sizeof(T) + j]);
		#endif
	}
	printf ("\n");
  }
}

// Matrix multiplication in software.
void mmul_sw(T a[L][M], T b[M][N], T out[L][N])
{
	for (int i = 0; i < L; ++i)
		for (int j = 0; j < N; ++j) {
			T sum = 0;
			for (int k = 0; k < M; ++k)
				sum += a[i][k] * b[k][j];
			out[i][j] = sum;
		}
}

int main()
{
	printf("Starting test...\n");

	hls::stream<T_SIDE> AB, C;
	T a[L][M], b[M][N], c_sw[L][N], c_hw[L][N];

	// Stream in matrix A.
	for(int i = 0; i < L; ++i)
		for(int j = 0; j < M; ++j) {
			a[i][j] = i + j + 1;
			AB << push_stream<T, T_SIDE>(a[i][j]);
		}

	// Stream in matrix B (transposed).
	for(int j = 0; j < N; ++j)
		for(int i = 0; i < M; ++i) {
			b[i][j] = (i * j) + 1;
			AB << push_stream<T, T_SIDE>(b[i][j]);
		}

	// Run software and hardware implementation.
	printf("Running software implementation...\n");
	mmul_sw(a, b, c_sw);
	printf("Running hardware implementation...\n");
	mmul_v3(AB, C, L, M, N);

	// Stream out matrix C (transposed).
	for(int j = 0; j < N; ++j)
		for(int i = 0; i < L; ++i)
			c_hw[i][j] = pop_stream<T, T_SIDE>(C.read());

	// Print out the results.
	printmat((T *)a, "a", L, M);
	printmat((T *)b, "b", M, N);
	printmat((T *)c_sw, "c_sw", L, N);
	printmat((T *)c_hw, "c_hw", L, N);

	// Check if software and hardware implementation match.
	int err = 0;
	for (int i = 0; (i < L && !err); ++i)
		for (int j = 0; (j < N && !err); ++j)
			if (c_sw[i][j] != c_hw[i][j])
				++err;

	if (!err) {
		printf("Test succeeded!\n");
		return 0;
	} else {
		printf("Test failed!\n");
		return -1;
	}
}
