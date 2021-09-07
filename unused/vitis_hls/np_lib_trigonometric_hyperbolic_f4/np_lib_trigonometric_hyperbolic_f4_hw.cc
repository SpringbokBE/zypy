/**
 *  @file    np_lib_trigonometric_hyperbolic_f4_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    22/04/2021
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

#include "np_lib_trigonometric_hyperbolic_f4.h"

union {
	unsigned int u4;
	float f4;
} c1, c2, c3;

void sin_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = hls::sinf(c1.f4);
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void cos_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = hls::cosf(c1.f4);
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void tan_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		t = c1.f4;
		c2.f4 = hls::sinf(t) / hls::cosf(t);
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arcsin_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = hls::atanf(c1.f4 / hls::sqrtf(1 - c1.f4 * c1.f4));
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arccos_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = (float) M_PI_2 - hls::atanf((c1.f4) / hls::sqrtf(1 - c1.f4 * c1.f4));
		out.data =c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arctan_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = hls::atanf(c1.f4);
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void sinh_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		t = c1.f4;
		c2.f4 = 0.5f * (hls::expf(t) - hls::expf(-t));
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void cosh_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		t = c1.f4;
		c2.f4 = 0.5f * (hls::expf(t) + hls::expf(-t));
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void tanh_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		t = c1.f4;
		c2.f4 = (hls::expf(2.0f * t) - 1.0f) / (hls::expf(2.0f * t) + 1.0f);
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arcsinh_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = hls::logf(c1.f4 + hls::sqrtf(c1.f4 * c1.f4 + 1));
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arccosh_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 = (c1.f4 >= 1.0f) ? hls::logf(c1.f4 + hls::sqrtf(c1.f4 * c1.f4 - 1)) : 0.0f;
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arctanh_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;
	float t;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		t = c1.f4;
		c2.f4 =  (hls::abs(t) < 1.0f) ? 0.5f * hls::logf((1.0f + t) / (1.0f - t)) : 0.0f;
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void arctan2_f4_hw(stream_t &x1s, stream_t &x2s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, x2, out;
	float t1, t2, tt;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		x2 = x2s.read();
		c1.u4 = x1.data;
		c2.u4 = x2.data;
		t1 = c1.f4;
		t2 = c2.f4;

		if(t1 > 0.0f) {
			c3.f4 = hls::atanf(t2 / t1);
		} else if(t1 < 0.0f) {
			if(t2 >= 0)
				c3.f4 = hls::atanf(t2 / t1) + (float) M_PI;
			else
				c3.f4 = hls::atanf(t2 / t1) - (float) M_PI;
		} else {
			if(t2 > 0)
				c3.f4 = 0.5f * (float) M_PI;
			else if (t2 < 0)
				c3.f4 = -0.5f * (float) M_PI;
			else
				c3.f4 = 0.0f;
		}

		out.data = c3.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1 or x2.last != 1);
}

void degrees_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 =  c1.f4 * 180.0f / (float) M_PI;
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

void radians_f4_hw(stream_t &x1s, stream_t &outs)
{
    #pragma HLS inline
	channel_t x1, out;

	do {
        #pragma HLS pipeline II=1
		x1 = x1s.read();
		c1.u4 = x1.data;
		c2.f4 =  c1.f4 * (float) M_PI / 180.0f;
		out.data = c2.u4;
		out.last = x1.last;
		outs.write(out);
	} while(x1.last != 1);
}

// Unwrap by changing deltas between values to 2*pi complement.
// For more information, see:
// https://github.com/numpy/numpy/blob/v1.20.0/numpy/lib/function_base.py#L1492-L1546
//void unwrap_f4_hw(stream_t &x1s, stream_t &outs)
//{
//    #pragma HLS inline
//	channel_t x1, out;
//	float cumsum = 0, a, b, t, t1, t2, t3, prev, diff;
//	bool first = true;
//
//	do {
//        #pragma HLS pipeline II=4
//		x1 = x1s.read();
//		c1.u4 = x1.data;
//
//		a = cumsum - 2.0f * (float) M_PI;
//		b = cumsum + 2.0f * (float) M_PI;
//
//		if (first){
//			diff = 0;
//			first = false;
//		} else
//			diff = c1.f4 - prev;
//
//		if (diff > (float) M_PI) {
//			cumsum = a;
//			t = c1.f4 + a;
//		} else if(diff <= (float) -M_PI) {
//			cumsum = b;
//			t = c1.f4 + b;
//		} else {
//
//			t = c1.f4 + cumsum;
//		}
//
//		prev = c1.f4;
//		c2.f4 = t;
//		out.data = c2.u4;
//		out.last = x1.last;
//		outs.write(out);
//	} while(x1.last != 1);
//}

void np_lib_trigonometric_f4_hw(stream_t &x1s, stream_t &x2s, stream_t &outs, const sel_t sel)
{
    #pragma HLS interface axis port=x1s
	#pragma HLS interface axis port=x2s
	#pragma HLS interface axis port=outs
	#pragma HLS interface s_axilite port=sel
	#pragma HLS interface s_axilite port=return

	#pragma HLS allocation instances=hls::atanf limit=1 function

	switch(sel){
		case 0x00:
			sin_f4_hw(x1s, outs);
			break;
		case 0x01:
			cos_f4_hw(x1s, outs);
			break;
		case 0x02:
			tan_f4_hw(x1s, outs);
			break;
		case 0x03:
			arcsin_f4_hw(x1s, outs);
			break;
		case 0x04:
			arccos_f4_hw(x1s, outs);
			break;
		case 0x05:
			arctan_f4_hw(x1s, outs);
			break;
		case 0x06:
			sinh_f4_hw(x1s, outs);
			break;
		case 0x07:
			cosh_f4_hw(x1s, outs);
			break;
		case 0x08:
			tanh_f4_hw(x1s, outs);
			break;
		case 0x09:
			arcsinh_f4_hw(x1s, outs);
			break;
		case 0x0a:
			arccosh_f4_hw(x1s, outs);
			break;
		case 0x0b:
			arctanh_f4_hw(x1s, outs);
			break;
		case 0x0c:
			arctan2_f4_hw(x1s, x2s, outs);
			break;
		case 0x0d:
			degrees_f4_hw(x1s, outs);
			break;
		case 0x0e:
			radians_f4_hw(x1s, outs);
			break;
//		default:
//			unwrap_f4_hw(x1s, outs);
	}
}
