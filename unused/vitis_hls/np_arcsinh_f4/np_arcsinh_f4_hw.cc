/**
 *  @file    np_arcsinh_f4_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    15/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Inverse hyperbolic sine element-wise.
 *  Single precision (f4) implementation.
 *
 */

#include <hls_math.h>

#include "np_arcsinh_f4.h"
#include "stream.h"

void np_arcsinh_f4_hw(stream_t &X1, stream_t &OUT, len_t stream_len)
{
    #pragma HLS INTERFACE axis port=X1
    #pragma HLS INTERFACE axis port=OUT
    #pragma HLS INTERFACE s_axilite port=stream_len
    #pragma HLS INTERFACE s_axilite port=return

    for(int i = 0; i < stream_len; i++) {
        #pragma HLS PIPELINE II=1
        np_t a = pop_stream<np_t, channel_t>(X1.read());
        OUT << push_stream<np_t, channel_t>(hls::asinh(a), i == (stream_len - 1));
    }
}