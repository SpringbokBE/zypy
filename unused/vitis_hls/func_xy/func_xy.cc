/**
 *  @file    func_xy.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP to calculate some more complex function. Makes use of the BRAM interface
 *	to allow for fast storage of the results.
 *
 */

#include <hls_math.h>

void func_xy(float xs[100], float y, float res[100]) {
	#pragma HLS INTERFACE bram port=res
	#pragma HLS INTERFACE s_axilite port=y bundle=CRTL_BUS
	#pragma HLS INTERFACE bram port=xs
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	for (int i = 0; i < 100; i++) {
		res[i] = (hls::sin(xs[i]) + hls::sqrtf(hls::expf(y))) / 2;
	}
}
