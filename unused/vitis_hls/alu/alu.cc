/**
 *  @file    alu.cc
 *  @author  Gerbrand De Laender
 *  @date    02/01/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that allows to add, subtract, multiply and divide.
 *
 */

void alu(int a, int b, int op, int * c) {
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=a bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=b bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=c bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=op bundle=CRTL_BUS

	switch (op) {
	case 0: {
		*c = a + b;
		break;
	}
	case 1: {
		*c = a - b;
		break;
	}
	case 2: {
		*c = a * b;
		break;
	}
	case 3: {
		*c = a / b;
		break;
	}
	}
}
