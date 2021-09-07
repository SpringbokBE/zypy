/**
 *  @file    distance.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP to that is able to calculate the Euclidean distance between two points,
 *  given their coordinates. Provides three versions: double, float and fixed
 *  point. This example shows that the fixed point implementation has the least
 *  resource usage.
 *
 */

#include <hls_math.h>
#include <ap_fixed.h>

typedef ap_fixed<16, 5> fixed;

double distance_double(double x1, double y1, double x2, double y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	return hls::sqrt(dx * dx + dy * dy);
}

float distance_float(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return hls::sqrtf(dx * dx + dy * dy);
}

fixed distance_fixed(fixed x1, fixed y1, fixed x2, fixed y2) {
	fixed dx = x2 - x1;
	fixed dy = y2 - y1;
	return hls::sqrt(dx * dx + dy * dy);
}
