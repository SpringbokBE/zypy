/**
 *  @file    gravity_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'gravity.cc'.
 *
 */

#include <stdio.h>

float reference[] = { 2000000000.0, 2000.0, 500.0, 222.22222222222223, 125.0,
		80.0, 55.55555555555556, 40.816326530612244, 31.25, 24.691358024691358 };
float gravity(float m1, float m2, float d);

int main() {
	float error = 0.0;

	for (int d = 0; d < 10; d++) {
		float force = gravity(10.0, 20.0, (float) d);
		error += force - reference[d];

		printf("gravity(10.0, 20.0, %f) = %f (accumulated error = %f)\n",
				(float) d, force, error);

		if (error > 1) {
			printf("Accumulated error too large! Aborting...\n");
			return 1;
		}
	}

	return 0;
}
