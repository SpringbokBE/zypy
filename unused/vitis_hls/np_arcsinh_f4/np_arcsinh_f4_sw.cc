/**
 *  @file    np_arcsinh_f4_sw.cc
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

void np_arcsinh_f4_sw(np_t *x1, np_t *out, len_t len)
{
	for(int i = 0; i < len; i++) {
		np_t a = x1[i];
		out[i] = hls::asinh(a);
	}
}
