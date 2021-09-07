/**
 *  @file    sum_of_squares_variable.cc
 *  @author  Gerbrand De Laender
 *  @date    18/02/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that calculates the sum of the squares of an incoming AXI stream.
 *
 */
#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_uint<32> T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

T sum_of_squares_variable(hls::stream<T_SIDE> &in_stream, ap_uint<8> stream_len_min_one)
{
	#pragma HLS INTERFACE axis port=in_stream
	#pragma HLS INTERFACE s_axilite port=stream_len_min_one bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	T_SIDE tmp;
	static T res = 0;
	#pragma HLS RESET variable=res

	for (int i = 0; i < stream_len_min_one + 1; i++) {
		#pragma HLS PIPELINE
		in_stream >> tmp;
		res += (tmp.data * tmp.data);
	}

	return res;
}
