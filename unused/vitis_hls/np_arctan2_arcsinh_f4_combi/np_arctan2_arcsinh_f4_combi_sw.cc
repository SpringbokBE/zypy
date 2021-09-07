/**
 *  @file    np_arctan2_arcsinh_f4_combi_sw.cc
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

void np_arctan2_f4_sw(np_t *x1, np_t *x2, np_t *out, len_t len)
{
	for(int i = 0; i < len; i++) {
		np_t a = x1[i], b = x2[i];
		out[i] = hls::atan2(a, b);
	}
}

void np_arcsinh_f4_sw(np_t *x1, np_t *out, len_t len)
{
	for(int i = 0; i < len; i++) {
		np_t a = x1[i];
		out[i] = hls::asinh(a);
	}
}

void np_arctan2_arcsinh_f4_combi_sw(np_t *x1, np_t *x2, np_t *out, len_t len, sel_t sel)
{
	 switch(sel){
	        case 0x00:
	            np_arctan2_f4_sw(x1, x2, out, len);
	            break;
	        default:
	            np_arcsinh_f4_sw(x1, out, len);
	    }
}
