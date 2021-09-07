/**
 *  @file    distance_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'distance.cc'.
 *
 */

#include <stdio.h>
#include <ap_fixed.h>

typedef ap_fixed<16, 5> fixed;

double distance_double(double x1, double y1, double x2, double y2);
float distance_float(float x1, float y1, float x2, float y2);
fixed distance_fixed(fixed x1, fixed y1, fixed x2, fixed y2);

int main() {
	double x1 = 1;
	double y1 = 2.5;
	double x2 = 0.5;
	double y2 = 4.5;

	printf("distance_double(%f, %f, %f, %f) = %f\n", x1, y1, x2, y2,
			(float) distance_double(x1, y1, x2, y2));
	printf("distance_float(%f, %f, %f, %f) = %f\n", x1, y1, x2, y2,
			(float) distance_float(x1, y1, x2, y2));
	printf("distance_fixed(%f, %f, %f, %f) = %f\n", x1, y1, x2, y2,
			(float) distance_fixed(x1, y1, x2, y2));

	return 0;
}
