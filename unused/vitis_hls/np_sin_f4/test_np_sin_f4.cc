/**
 *  @file    test_np_sin_f4_hw.cc
 *  @author  Gerbrand De Laender
 *  @date    04/05/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'np_sin_f4_hw.cc'.
 *
 */

#include <iostream>
#include <stdlib.h>

#include "np_sin_f4.h"
#include "test.h"

#define LEN 21 // Length of the stream for testing

union {
	unsigned int u4;
	float f4;
} c1;

int test_np_sin_f4_hw()
{
	std::cout << "Starting 'np_sin_f4_hw' test...\n";
	srand(123); // Set random number generator seed

	channel_t in, out;
	stream_t ins, outs;
	float ina[LEN], outa_sw[LEN], outa_hw[LEN];

	// Fill in 'ins' and 'ina'
	for(int i = 0; i < LEN; i++){
		c1.f4 = ina[i] = (float) rand() / (float) RAND_MAX;
		in.data = c1.u4;
		in.last = (i == LEN - 1) ? 1 : 0;
		ins.write(in);
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_sin_f4_sw(ina, outa_sw, LEN);
	std::cout << "Running hardware implementation...\n";
	np_sin_f4_hw(ins, outs);

	// Read out 'outs' into 'outa_hw'
	int i = 0;

	do {
		out = outs.read();
		c1.u4 = out.data;
		outa_hw[i++] = c1.f4;
	} while(out.last != 1);

	// Print out the results
	print_arr(ina, "ina", LEN);
	print_arr(outa_sw, "outa_sw", LEN);
	print_arr(outa_hw, "outa_hw", LEN);

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(outa_sw[i] != outa_hw[i]) {
			std::cout << "Test failed!\n";
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

int test_np_sin_2f4_hw()
{
	std::cout << "Starting 'np_sin_2f4_hw' test...\n";
	srand(123); // Set random number generator seed

	channel2_t in, out;
	stream2_t ins, outs;
	float ina[LEN], outa_sw[LEN], outa_hw[LEN];

	// Fill in 'ins' and 'ina'
	in.keep = 0x0;
	for(int i = 0; i < LEN; i++){
		c1.f4 = ina[i] = (float) rand() / (float) RAND_MAX;

		in.last = (i == LEN - 1) ? 1 : 0;

		switch(i % 2){
			case 0:
				in.data(31, 0) = c1.u4;
				in.keep(3, 0) = 0xf;
				break;
			case 1:
				in.data(63, 32) = c1.u4;
				in.keep(7, 4) = 0xf;
				break;
		}

		if(in.last or i % 2 == 1) {
			ins.write(in);
			in.keep = 0x0;
		}
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_sin_f4_sw(ina, outa_sw, LEN);
	std::cout << "Running hardware implementation...\n";
	np_sin_2f4_hw(ins, outs);

	// Read out 'outs' into 'outa_hw'
	int i = 0;

	do {
		out = outs.read();

		if(out.keep(3, 0) == 0xf) {
			c1.u4 = out.data.range(31, 0);
			outa_hw[i++] = c1.f4;
		}
		if(out.keep(7, 4) == 0xf) {
			c1.u4 = out.data.range(63, 32);
			outa_hw[i++] = c1.f4;
		}
	} while(out.last != 1);

	// Print out the results
	print_arr(ina, "ina", LEN);
	print_arr(outa_sw, "outa_sw", LEN);
	print_arr(outa_hw, "outa_hw", LEN);

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(outa_sw[i] != outa_hw[i]) {
			std::cout << "Test failed!\n";
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

int test_np_sin_4f4_hw()
{
	std::cout << "Starting 'np_sin_4f4_hw' test...\n";
	srand(123); // Set random number generator seed

	channel4_t in, out;
	stream4_t ins, outs;
	float ina[LEN], outa_sw[LEN], outa_hw[LEN];

	// Fill in 'ins' and 'ina'
	in.keep = 0x0;
	for(int i = 0; i < LEN; i++){
		c1.f4 = ina[i] = (float) rand() / (float) RAND_MAX;

		in.last = (i == LEN - 1) ? 1 : 0;

		switch(i % 4){
			case 0:
				in.data(31, 0) = c1.u4;
				in.keep(3, 0) = 0xf;
				break;
			case 1:
				in.data(63, 32) = c1.u4;
				in.keep(7, 4) = 0xf;
				break;
			case 2:
				in.data(95, 64) = c1.u4;
				in.keep(11, 8) = 0xf;
				break;
			case 3:
				in.data(127, 96) = c1.u4;
				in.keep(15, 12) = 0xf;
				break;
		}

		if(in.last or i % 4 == 3) {
			ins.write(in);
			in.keep = 0x0;
		}
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_sin_f4_sw(ina, outa_sw, LEN);
	std::cout << "Running hardware implementation...\n";
	np_sin_4f4_hw(ins, outs);

	// Read out 'outs' into 'outa_hw'
	int i = 0;

	do {
		out = outs.read();

		if(out.keep(3, 0) == 0xf) {
			c1.u4 = out.data.range(31, 0);
			outa_hw[i++] = c1.f4;
		}
		if(out.keep(7, 4) == 0xf) {
			c1.u4 = out.data.range(63, 32);
			outa_hw[i++] = c1.f4;
		}
		if(out.keep(11, 8) == 0xf) {
			c1.u4 = out.data.range(95, 64);
			outa_hw[i++] = c1.f4;
		}
		if(out.keep(15, 12) == 0xf) {
			c1.u4 = out.data.range(127, 96);
			outa_hw[i++] = c1.f4;
		}
	} while(out.last != 1);

	// Print out the results
	print_arr(ina, "ina", LEN);
	print_arr(outa_sw, "outa_sw", LEN);
	print_arr(outa_hw, "outa_hw", LEN);

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(outa_sw[i] != outa_hw[i]) {
			std::cout << "Test failed!\n";
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

int test_np_sin_8f4_hw()
{
	std::cout << "Starting 'np_sin_8f4_hw' test...\n";
	srand(123); // Set random number generator seed

	channel8_t in, out;
	stream8_t ins, outs;
	float ina[LEN], outa_sw[LEN], outa_hw[LEN];

	// Fill in 'ins' and 'ina'
	in.keep = 0x0;
	for(int i = 0; i < LEN; i++){
		c1.f4 = ina[i] = (float) rand() / (float) RAND_MAX;

		in.last = (i == LEN - 1) ? 1 : 0;

		switch(i % 8){
			case 0:
			  in.data(31, 0) = c1.u4;
			  in.keep(3, 0) = 0xf;
			  break;
			case 1:
			  in.data(63, 32) = c1.u4;
			  in.keep(7, 4) = 0xf;
			  break;
			case 2:
			  in.data(95, 64) = c1.u4;
			  in.keep(11, 8) = 0xf;
			  break;
			case 3:
			  in.data(127, 96) = c1.u4;
			  in.keep(15, 12) = 0xf;
			  break;
			case 4:
			  in.data(159, 128) = c1.u4;
			  in.keep(19, 16) = 0xf;
			  break;
			case 5:
			  in.data(191, 160) = c1.u4;
			  in.keep(23, 20) = 0xf;
			  break;
			case 6:
			  in.data(223, 192) = c1.u4;
			  in.keep(27, 24) = 0xf;
			  break;
			case 7:
			  in.data(255, 224) = c1.u4;
			  in.keep(31, 28) = 0xf;
			  break;
		}

		if(in.last or i % 8 == 7) {
			ins.write(in);
			in.keep = 0x0;
		}
	}

	// Run software and hardware implementation
	std::cout << "Running software implementation...\n";
	np_sin_f4_sw(ina, outa_sw, LEN);
	std::cout << "Running hardware implementation...\n";
	np_sin_8f4_hw(ins, outs);

	// Read out 'outs' into 'outa_hw'
	int i = 0;

	do {
		out = outs.read();

		if(out.keep(3, 0) == 0xf) {
		  c1.u4 = out.data.range(31, 0);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(7, 4) == 0xf) {
		  c1.u4 = out.data.range(63, 32);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(11, 8) == 0xf) {
		  c1.u4 = out.data.range(95, 64);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(15, 12) == 0xf) {
		  c1.u4 = out.data.range(127, 96);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(19, 16) == 0xf) {
		  c1.u4 = out.data.range(159, 128);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(23, 20) == 0xf) {
		  c1.u4 = out.data.range(191, 160);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(27, 24) == 0xf) {
		  c1.u4 = out.data.range(223, 192);
		  outa_hw[i++] = c1.f4;
		}
		if(out.keep(31, 28) == 0xf) {
		  c1.u4 = out.data.range(255, 224);
		  outa_hw[i++] = c1.f4;
		}
	} while(out.last != 1);

	// Print out the results
	print_arr(ina, "ina", LEN);
	print_arr(outa_sw, "outa_sw", LEN);
	print_arr(outa_hw, "outa_hw", LEN);

	// Check if software and hardware implementation match
	for(int i = 0; i < LEN; i++)
		if(outa_sw[i] != outa_hw[i]) {
			std::cout << "Test failed!\n";
			return -1;
		}

	std::cout << "Test succeeded!\n";
	return 0;
}

int main()
{
	test_np_sin_f4_hw();
	test_np_sin_2f4_hw();
	test_np_sin_4f4_hw();
	test_np_sin_8f4_hw();

	return 0;
}
