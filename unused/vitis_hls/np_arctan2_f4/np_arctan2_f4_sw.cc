/**
 *  @file    np_arctan2_f4_sw.cc
 *  @author  Gerbrand De Laender
 *  @date    07/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Element-wise arc tangent of x1/x2 choosing the quadrant correctly.
 *  Single precision (f4) implementation.
 *
 */

#include <hls_math.h>

#include "np_arctan2_f4.h"
#include "stream.h"

void np_arctan2_f4_sw(np_t *x1, np_t *x2, np_t *out, len_t len)
{
	for(int i = 0; i < len; i++) {
		np_t a = x1[i], b = x2[i];
		out[i] = hls::atan2(a, b);
	}
}
