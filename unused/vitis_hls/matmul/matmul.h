/**
 *  @file    matmul.h
 *  @author  Gerbrand De Laender
 *  @date    24/01/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Implementation of matrix multiplication using templates.
 *
 */

#include <hls_stream.h>

template<typename T, int DIM>
void matmul_hw(T a[DIM][DIM], T b[DIM][DIM], T out[DIM][DIM])
{
	#pragma HLS INLINE

//	const int FACTOR = DIM / 2;
	const int FACTOR = DIM / 4;
	#pragma HLS array_partition variable=a block factor=FACTOR dim=2
	#pragma HLS array_partition variable=b block factor=FACTOR dim=1

	L1:for (int j = 0; j < DIM; ++j)
		L2:for (int i = 0; i < DIM; ++i) {
			#pragma HLS PIPELINE II=1
			T sum = 0;
			L3:for (int k = 0; k < DIM; ++k)
				sum += a[i][k] * b[k][j];
			out[i][j] = sum;
		}
}

template <typename T, int DIM>
void matmul_sw(T a[DIM][DIM], T b[DIM][DIM], T out[DIM][DIM])
{
	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j) {
			T sum = 0;
			for (int k = 0; k < DIM; ++k)
				sum += a[i][k] * b[k][j];
			out[i][j] = sum;
		}
}
