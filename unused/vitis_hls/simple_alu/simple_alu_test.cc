/**
 *  @file    simple_alu_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'simple_alu.cc'.
 *
 */

#include <stdio.h>
#include <ap_int.h>

ap_uint<17> simple_alu(ap_uint<17> inA, ap_uint<17> inB, ap_uint<1> op);

int main() {
	ap_uint<17> inA, inB, resAdd, resSub;
	ap_uint<1> op;

	inA = 10;
	inB = 8;

	printf("Reference:\n");
	printf("%d + %d = %d\n", (unsigned int) inA, (unsigned int) inB,
			(unsigned int) inA + (unsigned int) inB);
	printf("%d - %d = %d\n\n", (unsigned int) inA, (unsigned int) inB,
			(unsigned int) inA - (unsigned int) inB);

	printf("HLS result:\n");
	printf("%d + %d = %d\n", (unsigned int) inA, (unsigned int) inB,
			(unsigned int) simple_alu(inA, inB, 1));
	printf("%d - %d = %d\n\n", (unsigned int) inA, (unsigned int) inB,
			(unsigned int) simple_alu(inA, inB, 0));

	return 0;
}
