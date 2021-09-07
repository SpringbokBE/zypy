/**
 *  @file    np_add_subtract_multiply_divide_f4_quad_sw.cc
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

void np_add_subtract_multiply_divide_f4_quad_sw(np_t *x1, np_t *x2, np_t *out, len_t len, sel_t sel)
{
	for(int i = 0; i < len; i++) {
		switch(sel) {
			case 0x00:
				out[i] = x1[i] + x2[i];
				break;
			case 0x01:
				out[i] = x1[i] - x2[i];
				break;
			case 0x02:
				out[i] = x1[i] * x2[i];
				break;
			default:
				out[i] = x1[i] / x2[i];
		}
	}
}
