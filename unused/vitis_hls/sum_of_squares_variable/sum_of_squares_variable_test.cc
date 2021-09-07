/**
 *  @file    sum_of_squares_variable_test.cc
 *  @author  Gerbrand De Laender
 *  @date    18/02/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'sum_of_squares.cc'.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define N 1234
typedef ap_uint<32> T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

T sum_of_squares_variable(hls::stream<T_SIDE> &in_stream, ap_uint<8> stream_len_min_one);

int main(){
	hls::stream<T_SIDE> in_stream;

	T res;
	int x = 0;
	int stream_len = 256;
	int iterations = N / stream_len;
	int remainder = N % stream_len;

	for(int i = 0; i < iterations; i++){
		for(int j = 0; j < stream_len; j++, x++){
			T_SIDE in;
			in.data = x;
			in.keep = 1;
			in.strb = 1;
			in.user = 1;
			in.id = 0;
			in.dest = 0;
			in_stream << in;
		}
		res = sum_of_squares_variable(in_stream, stream_len - 1);
	}

	if(remainder){
		for(int i = 0; i < remainder; i++, x++){
			T_SIDE in;
			in.data = x;
			in.keep = 1;
			in.strb = 1;
			in.user = 1;
			in.id = 0;
			in.dest = 0;
			in_stream << in;
		}

		res = sum_of_squares_variable(in_stream, remainder - 1);
	}

	printf("res = %d\n", static_cast<int>(res));

	return 0;
}
