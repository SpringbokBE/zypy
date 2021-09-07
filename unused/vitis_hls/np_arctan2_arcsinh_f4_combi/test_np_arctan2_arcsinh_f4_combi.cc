/**
 *  @file    test_np_arctan2_f4.cc
 *  @author  Gerbrand De Laender
 *  @date    07/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'np_arctan2_arcsinh_f4_combi.h'.
 *
 */

#include <iostream>
#include <stdlib.h>

#include "np_arctan2_arcsinh_f4_combi.h"
#include "stream.h"
#include "test.h"

#define LEN 16 // Length of the stream for testing

int test_np_arctan2_f4()
{
	std::cout << "Starting 'np_arctan2_f4' test...\n";
	srand(123); // Set random number generator seed

	stream_t X1, X2, OUT;
	np_t x1[LEN], x2[LEN], out_sw[LEN], out_hw[LEN], x1_tmp[LEN], x2_tmp[LEN];

	// Stream in X1 and X2
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand();
		x2[i] = (np_t) rand();
		X1 << push_stream<np_t, channel_t>(x1[i]);
		X2 << push_stream<np_t, channel_t>(x2[i]);
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_arctan2_arcsinh_f4_combi_sw(x1, x2, out_sw, LEN, 0x00);
	std::cout << "Running hardware implementation...\n";
	np_arctan2_arcsinh_f4_combi_hw(X1, X2, OUT, LEN, 0x00);

	// Stream out OUT
	for(int i = 0; i < LEN; i++)
		out_hw[i] = pop_stream<np_t, channel_t>(OUT.read());

	// Print out the results
	print_arr<np_t>(x1, "x1", LEN);
	print_arr<np_t>(x2, "x2", LEN);
	print_arr<np_t>(out_sw, "out_sw", LEN);
	print_arr<np_t>(out_hw, "out_hw", LEN);

	// Check if software and hardware implementation match
	bool err = 0;
	for(int i = 0; i < LEN && !err; i++)
		if(out_sw[i] != out_hw[i]) err = true;

	if(err){
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

int test_np_arcsinh_f4()
{
	std::cout << "Starting 'np_arcsinh_f4' test...\n";
	srand(123); // Set random number generator seed

	stream_t X1, OUT;
	np_t x1[LEN], out_sw[LEN], out_hw[LEN];

	// Stream in X1
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand();
		X1 << push_stream<np_t, channel_t>(x1[i]);
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_arctan2_arcsinh_f4_combi_sw(x1, x1, out_sw, LEN, 0x01);
	std::cout << "Running hardware implementation...\n";
	np_arctan2_arcsinh_f4_combi_hw(X1, X1, OUT, LEN, 0x01);

	// Stream out OUT
	for(int i = 0; i < LEN; i++)
		out_hw[i] = pop_stream<np_t, channel_t>(OUT.read());

	// Print out the results
	print_arr<np_t>(x1, "x1", LEN);
	print_arr<np_t>(out_sw, "out_sw", LEN);
	print_arr<np_t>(out_hw, "out_hw", LEN);

	// Check if software and hardware implementation match
	bool err = 0;
	for(int i = 0; i < LEN && !err; i++)
		if(out_sw[i] != out_hw[i]) err = true;

	if(err){
		std::cout << "Test failed!\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}

int main()
{
	test_np_arctan2_f4();
	test_np_arcsinh_f4();

	return 0;
}
