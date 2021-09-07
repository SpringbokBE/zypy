/**
 *  @file    np_sin_f4_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    04/05/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Single precision (f4) implementation.
 *
 */

#include <hls_math.h>
#include <math.h>

#include "np_sin_f4.h"

union {
	unsigned int u4;
	float f4;
} c1, c2;

void np_sin_f4_hw(stream_t &ins, stream_t &outs)
{
	#pragma HLS interface axis port=ins
	#pragma HLS interface axis port=outs
	#pragma HLS interface s_axilite port=return

	channel_t in, out;

	do {
        #pragma HLS pipeline II=1
		in = ins.read();
		out = in; // Copy side channels

		if(in.keep(3, 0) & 0xf) {
			c1.u4 = in.data.range(31, 0);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(31, 0) = c2.u4;
		}

		outs.write(out);
	} while(in.last != 1);
}

void np_sin_2f4_hw(stream2_t &ins, stream2_t &outs)
{
	#pragma HLS interface axis port=ins
	#pragma HLS interface axis port=outs
	#pragma HLS interface s_axilite port=return

	channel2_t in, out;

	do {
        #pragma HLS pipeline II=1
		in = ins.read();
		out = in; // Copy side channels

		if(in.keep(3, 0) & 0xf) {
			c1.u4 = in.data.range(31, 0);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(31, 0) = c2.u4;
		}
		if(in.keep(7, 4) & 0xf) {
			c1.u4 = in.data.range(63, 32);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(63, 32) = c2.u4;
		}

		outs.write(out);
	} while(in.last != 1);
}

void np_sin_4f4_hw(stream4_t &ins, stream4_t &outs)
{
	#pragma HLS interface axis port=ins
	#pragma HLS interface axis port=outs
	#pragma HLS interface s_axilite port=return

	channel4_t in, out;

	do {
        #pragma HLS pipeline II=1
		in = ins.read();
		out = in; // Copy side channels

		if(in.keep(3, 0) & 0xf) {
			c1.u4 = in.data.range(31, 0);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(31, 0) = c2.u4;
		}
		if(in.keep(7, 4) & 0xf) {
			c1.u4 = in.data.range(63, 32);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(63, 32) = c2.u4;
		}
		if(in.keep(11, 8) & 0xf) {
			c1.u4 = in.data.range(95, 64);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(95, 64) = c2.u4;
		}
		if(in.keep(15, 12) & 0xf) {
			c1.u4 = in.data.range(127, 96);
			c2.f4 = hls::sinf(c1.f4);
			out.data.range(127, 96) = c2.u4;
		}

		outs.write(out);
	} while(in.last != 1);
}

void np_sin_8f4_hw(stream8_t &ins, stream8_t &outs)
{
	#pragma HLS interface axis port=ins
	#pragma HLS interface axis port=outs
	#pragma HLS interface s_axilite port=return

	channel8_t in, out;

	do {
        #pragma HLS pipeline II=1
		in = ins.read();
		out = in; // Copy side channels

		if(in.keep(3, 0) == 0xf) {
		  c1.u4 = in.data.range(31, 0);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(31, 0) = c2.u4;
		}
		if(in.keep(7, 4) == 0xf) {
		  c1.u4 = in.data.range(63, 32);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(63, 32) = c2.u4;
		}
		if(in.keep(11, 8) == 0xf) {
		  c1.u4 = in.data.range(95, 64);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(95, 64) = c2.u4;
		}
		if(in.keep(15, 12) == 0xf) {
		  c1.u4 = in.data.range(127, 96);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(127, 96) = c2.u4;
		}
		if(in.keep(19, 16) == 0xf) {
		  c1.u4 = in.data.range(159, 128);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(159, 128) = c2.u4;
		}
		if(in.keep(23, 20) == 0xf) {
		  c1.u4 = in.data.range(191, 160);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(191, 160) = c2.u4;
		}
		if(in.keep(27, 24) == 0xf) {
		  c1.u4 = in.data.range(223, 192);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(223, 192) = c2.u4;
		}
		if(in.keep(31, 28) == 0xf) {
		  c1.u4 = in.data.range(255, 224);
		  c2.f4 = hls::sinf(c1.f4);
		  out.data.range(255, 224) = c2.u4;
		}
		outs.write(out);
	} while(in.last != 1);
}
