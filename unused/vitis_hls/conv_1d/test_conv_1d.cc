/**
 *  @file    test_conv_1d.cc
 *  @author  Gerbrand De Laender
 *  @date    31/03/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'conv_1d.cc'.
 *
 */

#include <stdio.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "stream.h"

#define MAX_DIM 64
#define L 8
#define M 4
#define N L + M - 1

typedef unsigned int T;
//#define typefloat
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void conv_1d(hls::stream<T_SIDE> &BA, hls::stream<T_SIDE> &C, unsigned int l, unsigned int m);

// Helper function to print out an array.
void printarr(T *mat, char *name, int n)
{
  printf ("%s = \n", name);
  for (int i = 0; i < n; i++) {
	#ifdef typefloat
		printf ("%5.2f  ", mat[i]);
	#elif defined typedouble
		printf ("%5.2lf  ", mat[i]);
	#else
		  printf ("%d  ", mat[i]);
	#endif
	printf ("\n");
  }
}

// Convolution in software.
void conv_1d_sw(T a[L], T b[M], T c[N])
{
  for(int i = 0; i < N; ++i) {
	  c[i] = 0;
	  const int j_min = (i >= M - 1)? i - (M - 1) : 0;
	  const int j_max = (i <  L - 1)? i : L - 1;
	  for(int j = j_min; j <= j_max; ++j)
		  c[i] += (a[j] * b[i - j]);
  }
}

int main()
{
	printf("Starting test...\n");

	hls::stream<T_SIDE> BA, C;
	T a[L], b[M], c_sw[N], c_hw[N];

	// Stream in array B.
	for(int i = 0; i < M; ++i) {
		b[i] = i + 1;
		BA << push_stream<T, T_SIDE>(b[i]);
	}

	// Stream in array A.
	for(int i = 0; i < L; ++i) {
		a[i] = i + 1;
		BA << push_stream<T, T_SIDE>(a[i]);
	}


	// Run software and hardware implementation.
	printf("Running software implementation...\n");
	conv_1d_sw(a, b, c_sw);
	printf("Running hardware implementation...\n");
	conv_1d(BA, C, L, M);

	// Stream out array C.
	for(int i = 0; i < N; ++i)
		c_hw[i] = pop_stream<T, T_SIDE>(C.read());

	// Print out the results.
	printarr((T *)a, "a", L);
	printarr((T *)b, "b", M);
	printarr((T *)c_sw, "c_sw", N);
	printarr((T *)c_hw, "c_hw", N);

	// Check if software and hardware implementation match.
	int err = 0;
	for (int i = 0; (i < N && !err); ++i)
		if (c_sw[i] != c_hw[i])
			++err;

	if (!err) {
		printf("Test succeeded!\n");
		return 0;
	} else {
		printf("Test failed!\n");
		return -1;
	}
}
