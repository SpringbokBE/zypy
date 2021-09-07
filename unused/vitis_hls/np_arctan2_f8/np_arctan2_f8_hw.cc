/**
 *  @file    np_arctan2_f8_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    07/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Element-wise arc tangent of x1/x2 choosing the quadrant correctly.
 *  Double precision (f8) implementation.
 *
 */

#include <hls_math.h>

#include "np_arctan2_f8.h"
#include "stream.h"

void np_arctan2_f8_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len)
{
	#pragma HLS INTERFACE axis port=X1
	#pragma HLS INTERFACE axis port=X2
	#pragma HLS INTERFACE axis port=OUT
	#pragma HLS INTERFACE s_axilite port=stream_len
	#pragma HLS INTERFACE s_axilite port=return

	for(int i = 0; i < stream_len; i++) {
		#pragma HLS PIPELINE II=10
		np_t a = pop_stream<np_t, channel_t>(X1.read());
		np_t b = pop_stream<np_t, channel_t>(X2.read());
		OUT << push_stream<np_t, channel_t>(hls::atan2(a, b), i == (stream_len - 1));
	}
}
