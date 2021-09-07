/**
 *  @file    test_np_lib_trigonometric_f4.cc
 *  @author  Gerbrand De Laender
 *  @date    07/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'np_lib_trigonometric_f4_hw.cc'.
 *
 */

#include <iostream>
#include <stdlib.h>

#include "np_lib_trigonometric_f4.h"
#include "test.h"

#define LEN 21 // Length of the stream for testing

union {
	unsigned int u4;
	float f4;
} c1;

int test_sin_f4_hw()
{
	std::cout << "Starting 'sin_f4_hw' test...\n";
	srand(123); // Set random number generator seed

	channel_t x1, out;
	stream_t x1s, outs;
	np_t x1a[LEN], out_swa[LEN], out_hwa[LEN];

	// Fill in 'x1s' and 'x1a'
	for(int i = 0; i < LEN; i++){
		c1.f4 = x1a[i] = (np_t) rand() / (np_t) RAND_MAX;
		x1.data = c1.u4;
		x1.last = (i == LEN - 1) ? 1 : 0;
		x1s.write(x1);
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_lib_trigonometric_f4_sw(x1a, x1a /* Don't care here */, out_swa, LEN, 0x00);
	std::cout << "Running hardware implementation...\n";
	np_lib_trigonometric_f4_hw(x1s, x1s /* Don't care here */, outs, 0x00);

	// Read out 'outs' into 'out_hwa'
	int i = 0;

	do {
		out = outs.read();
		c1.u4 = out.data;
		out_hwa[i++] = c1.f4;
	} while(out.last != 1);

	// Print out the results
	print_arr<np_t>(x1a, "x1a", LEN);
	print_arr<np_t>(out_swa, "out_swa", LEN);
	print_arr<np_t>(out_hwa, "out_hwa", LEN);

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(out_swa[i] != out_hwa[i]) {
			std::cout << "Test failed!\n";
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

int test_cos_f4_hw()
{
	std::cout << "Starting 'cos_f4_hw' test...\n";
	srand(123); // Set random number generator seed

	channel_t x1, out;
	stream_t x1s, outs;
	np_t x1a[LEN], out_swa[LEN], out_hwa[LEN];

	// Fill in 'x1s' and 'x1a'
	for(int i = 0; i < LEN; i++){
		c1.f4 = x1a[i] = (np_t) rand() / (np_t) RAND_MAX;
		x1.data = c1.u4;
		x1.last = (i == LEN - 1) ? 1 : 0;
		x1s.write(x1);
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_lib_trigonometric_f4_sw(x1a, x1a /* Don't care here */, out_swa, LEN, 0x01);
	std::cout << "Running hardware implementation...\n";
	np_lib_trigonometric_f4_hw(x1s, x1s /* Don't care here */, outs, 0x01);

	// Read out 'outs' into 'out_hwa'
	int i = 0;

	do {
		out = outs.read();
		c1.u4 = out.data;
		out_hwa[i++] = c1.f4;
	} while(out.last != 1);

	// Print out the results
	print_arr<np_t>(x1a, "x1a", LEN);
	print_arr<np_t>(out_swa, "out_swa", LEN);
	print_arr<np_t>(out_hwa, "out_hwa", LEN);

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(out_swa[i] != out_hwa[i]) {
			std::cout << "Test failed!\n";
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

int main()
{
	test_sin_f4_hw();
	test_cos_f4_hw();

	return 0;
}
