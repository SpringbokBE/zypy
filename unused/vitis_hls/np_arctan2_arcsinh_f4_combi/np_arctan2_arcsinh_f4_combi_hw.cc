/**
 *  @file    np_arctan2_arcsinh_f4_combi_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    19/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Combined component including:
 *  - Element-wise arc tangent of x1/x2 choosing the quadrant correctly.
 *  - Inverse hyperbolic sine element-wise.
 *  Single precision (f4) implementation.
 *
 */

#include <hls_math.h>

#include "np_arctan2_arcsinh_f4_combi.h"
#include "stream.h"

void np_arctan2_f4_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len)
{
	#pragma HLS inline

	for(int i = 0; i < stream_len; i++) {
		#pragma HLS PIPELINE II=1
		np_t a = pop_stream<np_t, channel_t>(X1.read());
		np_t b = pop_stream<np_t, channel_t>(X2.read());
		OUT << push_stream<np_t, channel_t>(hls::atan2(a, b), i == (stream_len - 1));
	}
}

void np_arcsinh_f4_hw(stream_t &X1, stream_t &OUT, len_t stream_len)
{
    #pragma HLS inline

    for(int i = 0; i < stream_len; i++) {
        #pragma HLS PIPELINE II=1
        np_t a = pop_stream<np_t, channel_t>(X1.read());
        OUT << push_stream<np_t, channel_t>(hls::asinh(a), i == (stream_len - 1));
    }
}

void np_arctan2_arcsinh_f4_combi_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len, sel_t sel)
{
    #pragma HLS INTERFACE axis port=X1
	#pragma HLS INTERFACE axis port=X2
	#pragma HLS INTERFACE axis port=OUT
	#pragma HLS INTERFACE s_axilite port=stream_len
	#pragma HLS INTERFACE s_axilite port=sel
	#pragma HLS INTERFACE s_axilite port=return

    switch(sel){
        case 0x00:
            np_arctan2_f4_hw(X1, X2, OUT, stream_len);
            break;
        default:
            np_arcsinh_f4_hw(X1, OUT, stream_len);
    }
}
