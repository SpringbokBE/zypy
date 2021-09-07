/**
 *  @file    test_np_add_subtract_multiply_divide_f4_quad.cc
 *  @author  Gerbrand De Laender
 *  @date    21/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'np_add_subtract_multiply_divide_f4_hw.cc'.
 *
 */

#include <iostream>
#include <stdlib.h>

#include "np_add_subtract_multiply_divide_f4_quad.h"
#include "test.h"

#define LEN 23 // Length of the stream for testing

typedef union {
	unsigned int u4;
	np_t f4;
} converter;

int main()
{
	srand(123); // Set random number generator seed

	len_t stream_len = LEN / 4 + (LEN % 4 != 0); // Ceil of integer division
	channel_t a, b, c; a.keep = 0; b.keep = 0; c.keep = 0;
	stream_t X1, X2, OUT;
	np_t x1[LEN], x2[LEN], out_sw[LEN], out_hw[LEN];
	converter c1, c2, c3;

	std::cout << "Starting test...\n";

	// Stream in X1 and X2
	for(int i = 0; i < LEN; i++) {
		x1[i] = (np_t) rand() / (np_t) RAND_MAX;
		x2[i] = (np_t) rand() / (np_t) RAND_MAX;

		if(i % 4 == 0) {
			c1.f4 = x1[i];
			c2.f4 = x2[i];
			a.data(31, 0) = c1.u4;
			b.data(31, 0) = c2.u4;
			a.keep(3, 0) = 0xf;
			b.keep(3, 0) = 0xf;
		} else if(i % 4 == 1) {
			c1.f4 = x1[i];
			c2.f4 = x2[i];
			a.data(63, 32) = c1.u4;
			b.data(63, 32) = c2.u4;
			a.keep(7, 4) = 0xf;
			b.keep(7, 4) = 0xf;
		} else if(i % 4 == 2) {
			c1.f4 = x1[i];
			c2.f4 = x2[i];
			a.data(95, 64) = c1.u4;
			b.data(95, 64) = c2.u4;
			a.keep(11, 8) = 0xf;
			b.keep(11, 8) = 0xf;
		} else if(i % 4 == 3) {
			c1.f4 = x1[i];
			c2.f4 = x2[i];
			a.data(127, 96) = c1.u4;
			b.data(127, 96) = c2.u4;
			a.keep(15, 12) = 0xf;
			b.keep(15, 12) = 0xf;

			X1 << a;
			X2 << b;
			a.keep = b.keep = 0x0000;
		}

		if(i == LEN - 1) {
			a.last = b.last = 1;
			X1 << a;
			X2 << b;
		}
	}

	print_arr(x1, "x1", LEN);
	print_arr(x2, "x2", LEN);

	// Run software and hardware implementation: case ADDITION
	std::cout << "Running software implementation case 'ADDITION'...\n";
	np_add_subtract_multiply_divide_f4_quad_sw(x1, x2, out_sw, LEN, 0x00);
	std::cout << "Running hardware implementation case 'ADDITION'...\n";
	np_add_subtract_multiply_divide_f4_quad_hw(X1, X2, OUT, stream_len, 0x00);

	// Stream out OUT
	int i = 0;

	do {
		c = OUT.read();
		if(c.keep(3, 0) == 0xf) {
			c3.u4 = c.data.range(31, 0);
			out_hw[i++] = c3.f4;
		}
		if(c.keep(7, 4) == 0xf)
			c3.u4 = c.data.range(63, 32);
			out_hw[i++] = c3.f4;
		if(c.keep(11, 8) == 0xf)
			c3.u4 = c.data.range(95, 64);
			out_hw[i++] = c3.f4;
		if(c.keep(15, 12) == 0xf)
			c3.u4 = c.data.range(127, 96);
			out_hw[i++] = c3.f4;
	} while(c.last != 1);

	print_arr(out_sw, "out_sw", LEN);
	print_arr(out_hw, "out_hw", LEN);

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
