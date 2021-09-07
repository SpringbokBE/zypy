/**
 *  @file    test_np_add_subtract_multiply_divide_i4.cc
 *  @author  Gerbrand De Laender
 *  @date    20/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'np_add_subtract_multiply_divide_i4_hw.cc'.
 *
 */

#include <iostream>
#include <stdlib.h>

#include "np_add_subtract_multiply_divide_i4.h"
#include "stream.h"
#include "test.h"

#define LEN 16 // Length of the stream for testing

int main()
{
	std::cout << "Starting test...\n";
	srand(123); // Set random number generator seed

	stream_t X1, X2, OUT;
	np_t x1[LEN], x2[LEN], out_sw[LEN], out_hw[LEN];

	// Stream in X1 and X2
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand();
		x2[i] = (np_t) rand();
		X1 << push_stream<np_t, channel_t>(x1[i]);
		X2 << push_stream<np_t, channel_t>(x2[i]);
	}

	// Run software and hardware implementation: case ADDITION
	std::cout << "Running software implementation...\n";
	np_add_subtract_multiply_divide_i4_sw(x1, x2, out_sw, LEN, 0x00);
	std::cout << "Running hardware implementation...\n";
	np_add_subtract_multiply_divide_i4_hw(X1, X2, OUT, LEN, 0x00);

	// Stream out OUT
	for(int i = 0; i < LEN; i++)
		out_hw[i] = pop_stream<np_t, channel_t>(OUT.read());

	// Check if software and hardware implementation match
	bool err = 0;
	for(int i = 0; i < LEN && !err; i++)
		if(out_sw[i] != out_hw[i]) err = true;

	if(err){
		std::cout << "Test failed!\n";
		return -1;
	}

	// Stream in X1 and X2
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand();
		x2[i] = (np_t) rand();
		X1 << push_stream<np_t, channel_t>(x1[i]);
		X2 << push_stream<np_t, channel_t>(x2[i]);
	}

	// Run software and hardware implementation: case SUBTRACTION
	std::cout << "Running software implementation...\n";
	np_add_subtract_multiply_divide_i4_sw(x1, x2, out_sw, LEN, 0x01);
	std::cout << "Running hardware implementation...\n";
	np_add_subtract_multiply_divide_i4_hw(X1, X2, OUT, LEN, 0x01);

	// Stream out OUT
	for(int i = 0; i < LEN; i++)
		out_hw[i] = pop_stream<np_t, channel_t>(OUT.read());

	// Check if software and hardware implementation match
	err = 0;
	for(int i = 0; i < LEN && !err; i++)
		if(out_sw[i] != out_hw[i]) err = true;

	if(err){
		std::cout << "Test failed!\n";
		return -1;
	}

	// Stream in X1 and X2
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand();
		x2[i] = (np_t) rand();
		X1 << push_stream<np_t, channel_t>(x1[i]);
		X2 << push_stream<np_t, channel_t>(x2[i]);
	}

	// Run software and hardware implementation: case MULTIPLICATION
	std::cout << "Running software implementation...\n";
	np_add_subtract_multiply_divide_i4_sw(x1, x2, out_sw, LEN, 0x02);
	std::cout << "Running hardware implementation...\n";
	np_add_subtract_multiply_divide_i4_hw(X1, X2, OUT, LEN, 0x02);

	// Stream out OUT
	for(int i = 0; i < LEN; i++)
		out_hw[i] = pop_stream<np_t, channel_t>(OUT.read());

	// Check if software and hardware implementation match
	err = 0;
	for(int i = 0; i < LEN && !err; i++)
		if(out_sw[i] != out_hw[i]) err = true;

	if(err){
		std::cout << "Test failed!\n";
		return -1;
	}

	// Stream in X1 and X2
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand();
		x2[i] = (np_t) rand();
		X1 << push_stream<np_t, channel_t>(x1[i]);
		X2 << push_stream<np_t, channel_t>(x2[i]);
	}

	// Run software and hardware implementation: case DIVISION
	std::cout << "Running software implementation...\n";
	np_add_subtract_multiply_divide_i4_sw(x1, x2, out_sw, LEN, 0x03);
	std::cout << "Running hardware implementation...\n";
	np_add_subtract_multiply_divide_i4_hw(X1, X2, OUT, LEN, 0x03);

	// Stream out OUT
	for(int i = 0; i < LEN; i++)
		out_hw[i] = pop_stream<np_t, channel_t>(OUT.read());

	// Print
	print_arr(out_sw, "out_sw", LEN);
	print_arr(out_hw, "out_hw", LEN);

	// Check if software and hardware implementation match
	err = 0;
	for(int i = 0; i < LEN && !err; i++)
		if(out_sw[i] != out_hw[i]) err = true;

	if(err){
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}
