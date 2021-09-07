/**
 *  @file    sum_of_squares.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that calculates the sum of the squares of an incoming AXI stream.
 *
 */

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define N 1024

// Side channels are TID, TUSR, TKEEP, TLAST.
// Helps interfacing with other cores e.g. TLAST for DMA.
typedef ap_axis<32, 1, 1, 1> AXI_T;
typedef hls::stream<AXI_T> STREAM_T;

ap_int<32> sum_of_squares(STREAM_T &A) {
	#pragma HLS INTERFACE axis port=A
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	AXI_T tmpA;
	ap_int<32> res = 0;

	for (int i = 0; i < N; i++) {
		#pragma HLS PIPELINE
		A >> tmpA;
		res += (tmpA.data * tmpA.data);
	}

	return res;
}
