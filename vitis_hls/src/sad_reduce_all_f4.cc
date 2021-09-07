/**
 * IP implementing the sum of absolute differences for one-dimensional
 * inputs (or flattened multi-dimensional inputs). Floating point (f4)
 * implementation.
 *
 * IMPORTANT:
 *  - The minimum input length is equal to the PIPE_DEPTH constant.
 *
 * IMPLEMENTATION:
 *  - To achieve the lowest possible latency, the core calculates a series
 *    of partial results "in parallel" (in fact, fully pipelined at II=1).
 *    After this stage, the array of partial results is further processed using
 *    a tree-like access pattern to produce the final output value.
 *
 * NOTES:
 *  - The addition of floating point numbers on computers is not associative,
 *    and the choice of how to associate an expression can have an effect on
 *    rounding error.
 */

////////////////////////////////////////////////////////////////////////////////

#include "sad_reduce_all_f4.h"
#include "hls_math.h"

////////////////////////////////////////////////////////////////////////////////

// Top level module implementing the reduce method of NumPy ufuncs for
// one-dimensional inputs (or flattened multi-dimensional inputs).

float sad_reduce_all_f4(
	stream_t &in1_s,	// Main AXI4-Stream input.
	stream_t &in2_s		// Second AXI4-Stream input.
	)
{
    #pragma HLS interface axis port=in1_s
    #pragma HLS interface axis port=in2_s
	#pragma HLS interface s_axilite port=return

	channel_t in1_c, in2_c;	// To temporarily hold side-channel information.

	static float res_p[PIPE_DEPTH]; // Contains the partial results.
	unsigned int p = 0; // Current index within the partial results.
	unsigned int n = 0; // Indicates the n-th input value.

	// This loop calculates the partial results.

	#pragma HLS dependence variable=res_p inter false

	main: do {
		#pragma HLS pipeline II=1

		// Read the input streams.

		in1_c = in1_s.read();
		in1.u4 = in1_c.data;

		in2_c = in2_s.read();
		in2.u4 = in2_c.data;

		// Apply both the element-wise operation and the reduction operation
		// and store the first partial result.

		float absdiff = hls::absf(in1.f4 - in2.f4);
		res_p[p] += absdiff;

		if(n++ < PIPE_DEPTH) res_p[p] = absdiff;

		p = (p == PIPE_DEPTH - 1 ? 0 : p + 1);
	} while(in1_c.last != 1);

	// This loop reduces the partial results using a tree-like access pattern.

	#pragma HLS array_partition variable=res_p complete

	fini: for(int depth = 0; depth < LOG2_PIPE_DEPTH; depth++) {
		#pragma HLS unroll
		for(int i = 0; i < PIPE_DEPTH; i += (2 << depth)) {
			res_p[i] += res_p[i + (1 << depth)];
		}
	}

	return res_p[0];
}

////////////////////////////////////////////////////////////////////////////////
