/**
 *  @file    mmul_v4.cc
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

#define MAX_DIM 64

typedef unsigned int T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void mmul_v4(hls::stream<T_SIDE> &AB, hls::stream<T_SIDE> &C, ap_uint<6> l, ap_uint<6> m, ap_uint<6> n)
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
			a[i][j] = AB.read().data;
		}

	B1: for(int j = 0; j < n; ++j)
		B2: for(int i = 0; i < m; ++i) {
			#pragma HLS PIPELINE II=1
			b[i][j] = AB.read().data;
		}

	T_SIDE C_SIDE;

	union {
		T input;
		int output;
	} converter;

	C1: for (int j = 0; j < n; ++j)
		C2: for (int i = 0; i < l; ++i) {
			#pragma HLS PIPELINE II=1
			T sum = 0;
			C3: for (int k = 0; k < MAX_DIM; ++k) {
				if(k >= m) break;
				sum += a[i][k] * b[k][j];
			}

			converter.input = sum;
			C_SIDE.data = converter.output;
			C_SIDE.keep = 0xf;  // A one for each byte of data
			C_SIDE.strb = 0xf;  // A one for each byte of data
			C_SIDE.user = 0;
			C_SIDE.last = i == (l - 1) and j == (n - 1) ? 1 : 0;
			C_SIDE.id = 0;
			C_SIDE.dest = 0;

			C.write(C_SIDE);
		}
}

