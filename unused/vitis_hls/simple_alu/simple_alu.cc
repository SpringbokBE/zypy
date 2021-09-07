/**
 *  @file    simple_alu.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that allows to add and subtract two 17-bit unsigned integers.
 *  Shows the usage of ap_uint<>.
 *
 */

#include <ap_int.h>

ap_uint<17> simple_alu(ap_uint<17> inA, ap_uint<17> inB, ap_uint<1> op) {
	if (op == 1)
		return inA + inB;
	else
		return inA - inB;
}
