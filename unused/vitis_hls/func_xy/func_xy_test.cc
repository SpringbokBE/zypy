/**
 *  @file    func_xy_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'func_xy.cc'.
 *
 */

#include <stdio.h>

void func_xy(float xs[100], float y, float res[100]);

int main() {

	float xs[100], res[100];

	for (int i = 0; i < 100; i++)
		xs[i] = i;

	func_xy(xs, 0.01f, res);

	for (int i = 0; i < 100; i++)
		printf("func_xy(%f, 0.01f) = %f\n", xs[i], res[i]);

	return 0;
}
