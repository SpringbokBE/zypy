/**
 *  @file    np_lib_trigonometric_f4_hyperbolic_sw.cc
 *  @author  Gerbrand De Laender
 *  @date    19/04/2021
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

#include "np_lib_trigonometric_hyperbolic_f4.h"

void sin_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::sinf(x1a[i]);
}

void cos_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::cosf(x1a[i]);
}

void tan_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::tanf(x1a[i]);
}

void arcsin_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::asinf(x1a[i]);
}

void arccos_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::acosf(x1a[i]);
}

void arctan_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::atanf(x1a[i]);
}

void sinh_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::sinhf(x1a[i]);
}

void cosh_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::coshf(x1a[i]);
}

void tanh_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::tanhf(x1a[i]);
}

void arcsinh_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::asinhf(x1a[i]);
}

void arccosh_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::acoshf(x1a[i]);
}

void arctanh_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::atanhf(x1a[i]);
}

void arctan2_f4_sw(np_t *x1a, np_t *x2a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::atan2f(x1a[i], x2a[i]);
}

void degrees_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = x1a[i] * 180.0f / (float) M_PI;
}

void radians_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = x1a[i] * (float) M_PI / 180.0f;
}


void np_lib_trigonometric_f4_sw(np_t *x1a, np_t *x2a, np_t *outa, len_t len, sel_t sel)
{
    switch(sel){
		case 0x00:
			sin_f4_sw(x1a, outa, len);
			break;
		case 0x01:
			cos_f4_sw(x1a, outa, len);
			break;
		case 0x02:
			tan_f4_sw(x1a, outa, len);
			break;
		case 0x03:
			arcsin_f4_sw(x1a, outa, len);
			break;
		case 0x04:
			arccos_f4_sw(x1a, outa, len);
			break;
		case 0x05:
			arctan_f4_sw(x1a, outa, len);
			break;
		case 0x06:
			sinh_f4_sw(x1a, outa, len);
			break;
		case 0x07:
			cosh_f4_sw(x1a, outa, len);
			break;
		case 0x08:
			tanh_f4_sw(x1a, outa, len);
			break;
		case 0x09:
			arcsinh_f4_sw(x1a, outa, len);
			break;
		case 0x0a:
			arccosh_f4_sw(x1a, outa, len);
			break;
		case 0x0b:
			arctanh_f4_sw(x1a, outa, len);
			break;
		case 0x0c:
			arctan2_f4_sw(x1a, x2a, outa, len);
			break;
		case 0x0d:
			degrees_f4_sw(x1a, outa, len);
			break;
		case 0x0e:
			radians_f4_sw(x1a, outa, len);
			break;
    }
}
