/**
 *  @file    test_np_lib_trigonometric_hyperbolic_f4.cc
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

#include "np_lib_trigonometric_hyperbolic_f4.h"
#include "test.h"

#define LEN 100 // Length of the stream for testing

union {
	unsigned int u4;
	float f4;
} c1;

int test(const std::string &name, sel_t sel, len_t n_inputs)
{
	std::cout << "Starting '" << name << "' test...\n";
	srand(123); // Set random number generator seed

	channel_t x1, x2, out;
	stream_t x1s, x2s, outs;
	np_t x1a[LEN], x2a[LEN], out_swa[LEN], out_hwa[LEN], deltaa[LEN];

	// Fill in 'x1s' and 'x1a'
	for(int i = 0; i < LEN; i++){
		c1.f4 = x1a[i] = (np_t) rand() / (np_t) RAND_MAX;
		x1.data = c1.u4;
		x1.last = (i == LEN - 1) ? 1 : 0;
		x1s.write(x1);
	}

	if(n_inputs > 1) {
		// Fill in 'x2s' and 'x2a'
		for(int i = 0; i < LEN; i++){
			c1.f4 = x1a[i] = (np_t) rand() / (np_t) RAND_MAX;
			x2.data = c1.u4;
			x2.last = (i == LEN - 1) ? 1 : 0;
			x2s.write(x2);
		}

		// Run software and hardware implementation
		std::cout << "Running software implementation...\n";
		np_lib_trigonometric_f4_sw(x1a, x2a, out_swa, LEN, sel);
		std::cout << "Running hardware implementation...\n";
		np_lib_trigonometric_f4_hw(x1s, x2s, outs, sel);
	} else {
		// Run software and hardware implementation
		std::cout << "Running software implementation...\n";
		np_lib_trigonometric_f4_sw(x1a, x1a, out_swa, LEN, sel);
		std::cout << "Running hardware implementation...\n";
		np_lib_trigonometric_f4_hw(x1s, x1s, outs, sel);
	}

	// Read out 'outs' into 'out_hwa'
	int i = 0;

	do {
		out = outs.read();
		c1.u4 = out.data;
		out_hwa[i++] = c1.f4;
	} while(out.last != 1);

	float delta, max_delta = 0;

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(out_swa[i] != out_hwa[i]) {
			delta = abs(out_swa[i] - out_hwa[i]);
			deltaa[i] = delta;
			if(delta > max_delta)
				max_delta = delta;
		}

	// Print out the results
	print_arr<np_t>(x1a, "x1a", LEN);
	print_arr<np_t>(out_swa, "out_swa", LEN);
	print_arr<np_t>(out_hwa, "out_hwa", LEN);
	print_arr<np_t>(deltaa, "deltaa", LEN);

	if(max_delta) {
		std::cout << "Test failed! (max_delta = " << max_delta << ")\n";
		return -1;
	}

	std::cout << "Test succeeded!\n";
	return 0;
}


int main()
{
	test("sin_f4", 0x00, 1);
	test("cos_f4", 0x01, 1);
	test("tan_f4", 0x02, 1);
	test("arcsin_f4", 0x03, 1);
	test("arccos_f4", 0x04, 1);
	test("arctan_f4", 0x05, 1);
	test("sinh_f4", 0x06, 1);
	test("cosh_f4", 0x07, 1);
	test("tanh_f4", 0x08, 1);
	test("arcsinh_f4", 0x09, 1);
	test("arccosh_f4", 0x0a, 1);
	test("arctanh_f4", 0x0b, 1);
	test("arctan2_f4", 0x0c, 2);
	test("degrees_f4", 0x0d, 1);
	test("radians_f4", 0x0e, 1);

	return 0;
}

