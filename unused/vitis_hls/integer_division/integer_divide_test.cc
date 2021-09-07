/**
 *  @file    integer_divide_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'integer_divide.cc'.
 *
 */

#include <stdio.h>

void integer_divide(unsigned char N, unsigned char D, unsigned char *Q,
		unsigned char *R);

int main() {
	unsigned char N, D, Q, R;

	N = 8;
	D = 3;
	integer_divide(N, D, &Q, &R);
	printf("Division of %d/%d = %d r %d\n", N, D, Q, R);

	N = 4;
	D = 2;
	integer_divide(N, D, &Q, &R);
	printf("Division of %d/%d = %d r %d\n", N, D, Q, R);

	return 0;
}
