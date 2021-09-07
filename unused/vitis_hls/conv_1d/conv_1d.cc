/**
 *  @file    conv_1d.cc
 *  @author  Gerbrand De Laender
 *  @date    31/03/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  1D convolution of arrays A and B with storage of B.
 *
 */

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <assert.h>

#include "stream.h"

#define MAX_DIM 4096

typedef unsigned int T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void conv_1d(hls::stream<T_SIDE> &BA, hls::stream<T_SIDE> &C, unsigned int l, unsigned int m)
{
	#pragma HLS INTERFACE axis port=BA
	#pragma HLS INTERFACE axis port=C
	#pragma HLS INTERFACE s_axilite port=l
	#pragma HLS INTERFACE s_axilite port=m
	#pragma HLS INTERFACE s_axilite port=return

	assert(l <= MAX_DIM);
	assert(m <= MAX_DIM);

	T a[MAX_DIM], b[MAX_DIM];
	const unsigned int n = l + m - 1;

	B1:for(unsigned int i = 0; i < m; ++i){
		#pragma HLS PIPELINE II=1
		b[i] = pop_stream<T, T_SIDE>(BA.read());
	}

	A1:for(unsigned int i = 0; i < l; ++i) {
		#pragma HLS PIPELINE II=1
		a[i] = pop_stream<T, T_SIDE>(BA.read());
	}

//	C1: for(int i = 0; i < n; i++) {
//		#pragma HLS PIPELINE II=1
//		T sum = 0;
//
//		if(i < m) {
//			C2: for(int j = i; j >= 0; j--) {
//				#pragma HLS latency max=10
//				sum += a[i - j] * b[j];
//			}
//			C << push_stream<T, T_SIDE>(sum, 0);
//		} else if (i < l) {
//			C3: for(int j = m - 1; j >= 0; j--) {
//				#pragma HLS latency max=10
//				sum += a[i - j] * b[j];
//			}
//			C << push_stream<T, T_SIDE>(sum, 0);
//		} else {
//			C4: for(int j = i - l + 1; j < m; j++) {
//				#pragma HLS latency max=10
//				sum += a[i - j] * b[j];
//			}
//			C << push_stream<T, T_SIDE>(sum, i == (n - 1));
//		}
//	}

	C1: for(int i = 0; i < n; i++) {
		#pragma HLS PIPELINE II=64
		T sum = 0;

		if(i < m) {
			C2: for(int j = MAX_DIM - 1; j >= 0; j--) {
				#pragma HLS unroll factor=16
				if(j > i) continue;
				sum += a[i - j] * b[j];
			}
			C << push_stream<T, T_SIDE>(sum, 0);
		} else if (i < l) {
			C3: for(int j = MAX_DIM - 1; j >= 0; j--) {
				#pragma HLS unroll factor=16
				if (j > m - 1) continue;
				sum += a[i - j] * b[j];
			}
			C << push_stream<T, T_SIDE>(sum, 0);
		} else {
			C4: for(int j = 1; j < MAX_DIM; j++) {
				#pragma HLS unroll factor=16
				if(j <= i - l || j > m) continue;
				sum += a[i - j] * b[j];
			}
			C << push_stream<T, T_SIDE>(sum, i == (n - 1));
		}
	}
}
