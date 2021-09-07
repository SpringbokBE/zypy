/**
 *  @file    np_sin_f4_sw.cc
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

#include "np_sin_f4.h"

void np_sin_f4_sw(float *ina, float *outa, len_t len)
{
	for(int i = 0; i < len; i++)
		outa[i] = hls::sinf(ina[i]);
}
