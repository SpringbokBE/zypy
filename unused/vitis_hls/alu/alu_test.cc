/**
 *  @file    alu_test.cc
 *  @author  Gerbrand De Laender
 *  @date    02/01/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'alu.cc'.
 *
 */

#include <stdio.h>

void alu(int a, int b, int op, int * c);

int main() {

	int a = 2;
	int b = 3;
	int c;

	alu(a, b, 0, &c);
	printf("%d + %d = %d\n", a, b, c);

	alu(a, b, 1, &c);
	printf("%d - %d = %d\n", a, b, c);

	alu(a, b, 2, &c);
	printf("%d * %d = %d\n", a, b, c);

	alu(a, b, 3, &c);
	printf("%d / %d = %d\n", a, b, c);

	return 0;
}
