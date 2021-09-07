/**
 *  @file    np_add_subtract_multiply_divide_f4_quad_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    21/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Element-wise addition, subtraction, multiplication and division.
 *  Single precision (f4) implementation.
 *
 */

#include <hls_math.h>

#include "np_add_subtract_multiply_divide_f4_quad.h"

typedef union {
	unsigned int u4;
	np_t f4;
} converter;

void np_add_subtract_multiply_divide_f4_quad_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len, sel_t sel)
{
	#pragma HLS INTERFACE axis port=X1
	#pragma HLS INTERFACE axis port=X2
	#pragma HLS INTERFACE axis port=OUT
	#pragma HLS INTERFACE s_axilite port=stream_len
	#pragma HLS INTERFACE s_axilite port=sel
	#pragma HLS INTERFACE s_axilite port=return

	for(int i = 0; i < stream_len; i++) {
		#pragma HLS PIPELINE II=1
		channel_t x1 = X1.read();
		channel_t x2 = X2.read();
		channel_t out;
		converter c1, c2,c3;

		switch(sel) {
			case 0x00:
				if(x1.keep(3, 0) & 0xf) {
					c1.u4 = x1.data.range(31, 0);
					c2.u4 = x2.data.range(31, 0);
					c3.f4 = c1.f4 + c2.f4;
					out.data.range(31, 0) = c3.u4;
				}
				if(x1.keep(7, 4) & 0xf) {
					c1.u4 = x1.data.range(63, 32);
					c2.u4 = x2.data.range(63, 32);
					c3.f4 = c1.f4 + c2.f4;
					out.data.range(63, 32) = c3.u4;
				}
				if(x1.keep(11, 8) & 0xf) {
					c1.u4 = x1.data.range(95, 64);
					c2.u4 = x2.data.range(95, 64);
					c3.f4 = c1.f4 + c2.f4;
					out.data.range(95, 64) = c3.u4;
				}
				if(x1.keep(15, 12) & 0xf) {
					c1.u4 = x1.data.range(127, 96);
					c2.u4 = x2.data.range(127, 96);
					c3.f4 = c1.f4 + c2.f4;
					out.data.range(127, 96) = c3.u4;
				}
				break;
			case 0x01:
				if(x1.keep(3, 0) & 0xf) {
					c1.u4 = x1.data.range(31, 0);
					c2.u4 = x2.data.range(31, 0);
					c3.f4 = c1.f4 - c2.f4;
					out.data.range(31, 0) = c3.u4;
				}
				if(x1.keep(7, 4) & 0xf) {
					c1.u4 = x1.data.range(63, 32);
					c2.u4 = x2.data.range(63, 32);
					c3.f4 = c1.f4 - c2.f4;
					out.data.range(63, 32) = c3.u4;
				}
				if(x1.keep(11, 8) & 0xf) {
					c1.u4 = x1.data.range(95, 64);
					c2.u4 = x2.data.range(95, 64);
					c3.f4 = c1.f4 - c2.f4;
					out.data.range(95, 64) = c3.u4;
				}
				if(x1.keep(15, 12) & 0xf) {
					c1.u4 = x1.data.range(127, 96);
					c2.u4 = x2.data.range(127, 96);
					c3.f4 = c1.f4 - c2.f4;
					out.data.range(127, 96) = c3.u4;
				}
				break;
			case 0x02:
				if(x1.keep(3, 0) & 0xf) {
					c1.u4 = x1.data.range(31, 0);
					c2.u4 = x2.data.range(31, 0);
					c3.f4 = c1.f4 * c2.f4;
					out.data.range(31, 0) = c3.u4;
				}
				if(x1.keep(7, 4) & 0xf) {
					c1.u4 = x1.data.range(63, 32);
					c2.u4 = x2.data.range(63, 32);
					c3.f4 = c1.f4 * c2.f4;
					out.data.range(63, 32) = c3.u4;
				}
				if(x1.keep(11, 8) & 0xf) {
					c1.u4 = x1.data.range(95, 64);
					c2.u4 = x2.data.range(95, 64);
					c3.f4 = c1.f4 * c2.f4;
					out.data.range(95, 64) = c3.u4;
				}
				if(x1.keep(15, 12) & 0xf) {
					c1.u4 = x1.data.range(127, 96);
					c2.u4 = x2.data.range(127, 96);
					c3.f4 = c1.f4 * c2.f4;
					out.data.range(127, 96) = c3.u4;
				}
				break;
			default:
				if(x1.keep(3, 0) & 0xf) {
					c1.u4 = x1.data.range(31, 0);
					c2.u4 = x2.data.range(31, 0);
					c3.f4 = c1.f4 / c2.f4;
					out.data.range(31, 0) = c3.u4;
				}
				if(x1.keep(7, 4) & 0xf) {
					c1.u4 = x1.data.range(63, 32);
					c2.u4 = x2.data.range(63, 32);
					c3.f4 = c1.f4 / c2.f4;
					out.data.range(63, 32) = c3.u4;
				}
				if(x1.keep(11, 8) & 0xf) {
					c1.u4 = x1.data.range(95, 64);
					c2.u4 = x2.data.range(95, 64);
					c3.f4 = c1.f4 / c2.f4;
					out.data.range(95, 64) = c3.u4;
				}
				if(x1.keep(15, 12) & 0xf) {
					c1.u4 = x1.data.range(127, 96);
					c2.u4 = x2.data.range(127, 96);
					c3.f4 = c1.f4 / c2.f4;
					out.data.range(127, 96) = c3.u4;
				}
		}

		out.keep = x1.keep;

		if(i == stream_len - 1)
			out.last = 1;

		OUT << out;
	}
}
