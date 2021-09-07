/**
 *  @file    matmul_test.cc
 *  @author  Gerbrand De Laender
 *  @date    25/01/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'matmul.cc'.
 *
 */
#include <stdio.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "matmul.h"
#include "stream.h"

#define MAT_DIM 32
#define MAT_SIZE (MAT_DIM * MAT_DIM)

typedef float T;
typedef ap_axiu<8 * sizeof(T), 1, 1, 1> T_SIDE;

void matmul(hls::stream<T_SIDE> &in_stream, hls::stream<T_SIDE> &out_stream);

int main() {
	printf("Starting test...\n");

	T a[MAT_DIM][MAT_DIM];
	T b[MAT_DIM][MAT_DIM];
	T c_sw[MAT_DIM][MAT_DIM];
	T c_hw[MAT_DIM][MAT_DIM];
	T_SIDE tmp;

	hls::stream<T_SIDE> in_stream, out_stream;

	// Fill matrices.
	for(int i = 0; i < MAT_DIM; ++i)
		for(int j = 0; j < MAT_DIM; ++j) {
			a[i][j] = i + j;
			b[i][j] = i * j;
		}

	// Run software implementation.
	printf("Running software implementation...\n");
	matmul_sw(a, b, c_sw);

	// Fill AXI input stream with matrix A.
		for(int i = 0; i < MAT_DIM; ++i)
			for(int j = 0; j < MAT_DIM; ++j) {
				tmp = push_stream<T, T_SIDE>(a[i][j]);
				in_stream << tmp;
			}

	// Fill AXI input stream with matrix B.
		for(int j = 0; j < MAT_DIM; ++j)
			for(int i = 0; i < MAT_DIM; ++i) {
				tmp = push_stream<T, T_SIDE>(b[i][j], i == (MAT_DIM - 1) and j == (MAT_DIM - 1));
				in_stream << tmp;
			}

	// Run hardware implementation.
	printf("Running hardware implementation...\n");
	matmul(in_stream, out_stream);

	// Get C from the AXI output stream.
	for(int j = 0; j < MAT_DIM; ++j)
		for(int i = 0; i < MAT_DIM; ++i) {
			out_stream >> tmp;
			c_hw[i][j] = pop_stream<T, T_SIDE>(tmp);
	}

	// Compare results.
	int err = 0;
	for (int i = 0; (i < MAT_DIM && !err); ++i)
		for (int j = 0; (j < MAT_DIM && !err); ++j)
			if (c_sw[i][j] != c_hw[i][j])
				++err;

	if (!err) {
		printf("Test succeeded!\n");
		return 0;
	} else {
		printf("Test failed!\n");
		return -1;
	}
}
