/**
 *  @file    np_lib_trigonometric_f4_sw.cc
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

#include "np_lib_trigonometric_f4.h"

void sin_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::sin(x1a[i]);
}

void cos_f4_sw(np_t *x1a, np_t *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::cos(x1a[i]);
}

void np_lib_trigonometric_f4_sw(np_t *x1a, np_t *x2a, np_t *outa, len_t len, sel_t sel)
{
    switch(sel){
    case 0x00:
		sin_f4_sw(x1a, outa, len);
		break;
	default:
		cos_f4_sw(x1a, outa, len);
    }
}
