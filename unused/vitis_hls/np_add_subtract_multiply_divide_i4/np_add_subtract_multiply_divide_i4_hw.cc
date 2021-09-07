/**
 *  @file    np_add_subtract_multiply_divide_i4_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    20/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Element-wise addition, subtraction, multiplication and division.
 *  Signed integer (i4) implementation.
 *
 */

#include <hls_math.h>

#include "np_add_subtract_multiply_divide_i4.h"
#include "stream.h"

void np_add_subtract_multiply_divide_i4_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len, sel_t sel)
{
	#pragma HLS INTERFACE axis port=X1
	#pragma HLS INTERFACE axis port=X2
	#pragma HLS INTERFACE axis port=OUT
	#pragma HLS INTERFACE s_axilite port=stream_len
	#pragma HLS INTERFACE s_axilite port=sel
	#pragma HLS INTERFACE s_axilite port=return

	for(int i = 0; i < stream_len; i++) {
		#pragma HLS PIPELINE II=1
		np_t a = pop_stream<np_t, channel_t>(X1.read());
		np_t b = pop_stream<np_t, channel_t>(X2.read());
		switch(sel) {
			case 0x00:
				OUT << push_stream<np_t, channel_t>(a + b, i == (stream_len - 1));
				break;
			case 0x01:
				OUT << push_stream<np_t, channel_t>(a - b, i == (stream_len - 1));
				break;
			case 0x02:
				OUT << push_stream<np_t, channel_t>(a * b, i == (stream_len - 1));
				break;
			default:
				OUT << push_stream<np_t, channel_t>(a / b, i == (stream_len - 1));
		}
	}
}
