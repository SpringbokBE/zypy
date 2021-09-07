/**
 *  @file    mmul_v3.cc
 *  @author  Gerbrand De Laender
 *  @date    11/03/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Matrix multiplication with combined streaming of matrix A and B.
 *
 */

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "stream.h"

#define MAX_DIM 64

typedef unsigned int T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void mmul_v3(hls::stream<T_SIDE> &AB, hls::stream<T_SIDE> &C, ap_uint<6> l, ap_uint<6> m, ap_uint<6> n)
{
	#pragma HLS INTERFACE axis port=AB
	#pragma HLS INTERFACE axis port=C
	#pragma HLS INTERFACE s_axilite port=l
	#pragma HLS INTERFACE s_axilite port=m
	#pragma HLS INTERFACE s_axilite port=n
	#pragma HLS INTERFACE s_axilite port=return

	T a[MAX_DIM][MAX_DIM], b[MAX_DIM][MAX_DIM];

	const int FACTOR = MAX_DIM / 2;
	#pragma HLS array_partition block factor=FACTOR variable=a dim=2
	#pragma HLS array_partition block factor=FACTOR variable=b dim=1

	A1: for(int i = 0; i < l; ++i)
		A2: for(int j = 0; j < m; ++j) {
			#pragma HLS PIPELINE II=1
			a[i][j] = pop_stream<T, T_SIDE>(AB.read());
		}

	B1: for(int j = 0; j < n; ++j)
		B2: for(int i = 0; i < m; ++i) {
			#pragma HLS PIPELINE II=1
			b[i][j] = pop_stream<T, T_SIDE>(AB.read());
		}

	C1: for (int j = 0; j < n - 1; ++j)
		C2: for (int i = 0; i < l; ++i) {
			#pragma HLS PIPELINE II=1
			T sum = 0;
			C3: for (int k = 0; k < MAX_DIM; ++k) {
				if(k >= m) break;
				sum += a[i][k] * b[k][j];
			}
			C << push_stream<T, T_SIDE>(sum, 0);
		}

	C4: for (int i = 0; i < l - 1; ++i) {
		#pragma HLS PIPELINE II=1
		T sum = 0;
		C5: for (int k = 0; k < MAX_DIM; ++k) {
			if(k >= m) break;
			sum += a[i][k] * b[k][n - 1];
		}
		C << push_stream<T, T_SIDE>(sum, 0);
	}

	T sum = 0;
	C6: for (int k = 0; k < MAX_DIM; ++k) {
		if(k >= m) break;
		sum += a[l - 1][k] * b[k][n - 1];
	}
	C << push_stream<T, T_SIDE>(sum, 1);
}

