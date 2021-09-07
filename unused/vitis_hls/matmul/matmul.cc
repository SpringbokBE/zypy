/**
 *  @file    matmul.cc
 *  @author  Gerbrand De Laender
 *  @date    24/01/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Matrix multiplication.
 *
 */

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "matmul.h"
#include "stream.h"

#define MAT_DIM 32
#define MAT_SIZE (MAT_DIM * MAT_DIM)

typedef float T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void matmul_(hls::stream<T_SIDE> &in_stream, hls::stream<T_SIDE> &out_stream, T a[MAT_DIM][MAT_DIM])
{
	#pragma HLS DATAFLOW

	T b[MAT_DIM][MAT_DIM];
	T c[MAT_DIM][MAT_DIM];

	stream_in_matrix_column_wise<T, T_SIDE, MAT_DIM>(b, in_stream);

	matmul_hw<T, MAT_DIM>(a, b, c);

	stream_out_matrix_column_wise<T, T_SIDE, MAT_DIM>(c, out_stream);
}

void matmul(hls::stream<T_SIDE> &in_stream, hls::stream<T_SIDE> &out_stream)
{
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS INTERFACE axis port=in_stream
	#pragma HLS INTERFACE axis port=out_stream

	T a[MAT_DIM][MAT_DIM];
	stream_in_matrix_row_wise<T, T_SIDE, MAT_DIM>(a, in_stream);

	matmul_(in_stream, out_stream, a);
}

