/**
 * IP implementing the reduce method of NumPy ufuncs for one-dimensional
 * inputs (or flattened multi-dimensional inputs). Integer (i4) implementation.
 *
 * The reduction operator can be selected using the `args.reduce_op` parameter.
 * It is also possible to do an additional unary or binary operation on each
 * (pair of) incoming elements, using the `args.element_op` parameter, which
 * defaults to NONE (i.e. don't do any element-wise operation).
 *
 * IMPORTANT:
 *  - The reduction operation is assumed to be commutative (because the
 *    core will reorder some terms).
 *  - The minimum input length is equal to the PIPE_DEPTH constant.
 *  - If there are two input streams, the args.element_op needs to be a binary
 *    operation.
 *
 * EXAMPLES:
 * 	- To calculate the sum of all numbers of a single input stream:
 * 		> args.reduce_op = ADD
 * 		> args.element_op = NONE
 *	- To calculate the sum of the squares of a single input stream:
 *	    > args.reduce_op = ADD
 *	    > args.element_op = SQUARE
 *	- To calculate the dot product of two input streams:
 *	    > args.reduce_op = ADD
 *	    > args.element_op = MULTIPLY
 *
 * IMPLEMENTATION:
 *  - To achieve the lowest possible latency, the core calculates a series
 *    of partial results "in parallel" (in fact, fully pipelined at II=1).
 *    After this stage, the array of partial results is further processed using
 *    a tree-like access pattern to produce the final output value.
 *
 */

////////////////////////////////////////////////////////////////////////////////

#include "ufunc_reduce_all_i4.h"
#include "ufuncs_i4.h"

////////////////////////////////////////////////////////////////////////////////

// Function that selects and performs the required (reduction or element-wise)
// operation on inputs x and y. The result is stored back in x.

void do_operation(unsigned char *op, int *x, int *y)
{
	int2 ufunc_in, ufunc_out; // To temporarily hold the inputs and outputs.

	ufunc_in.first = *x;
	ufunc_in.second = *y;

	switch(*op) {
		case NONE:
			ufunc_out = ufunc_in;
			break;
		case ADD:
			ufunc_out = add(ufunc_in);
			break;
		case MULTIPLY:
			ufunc_out = multiply(ufunc_in);
			break;
		case SQUARE:
			ufunc_out = square(ufunc_in);
			break;
		case EXP:
			ufunc_out = exp(ufunc_in);
			break;
		case LOG:
			ufunc_out = log(ufunc_in);
			break;
		case ABSDIFF:
			ufunc_out = absdiff(ufunc_in);
			break;
	}

	*x = ufunc_out.first;
}

////////////////////////////////////////////////////////////////////////////////

// Top level module implementing the reduce method of NumPy ufuncs for
// one-dimensional inputs (or flattened multi-dimensional inputs).

int ufunc_reduce_all_i4(
	stream_t &in1_s,	// Main AXI4-Stream input.
	stream_t &in2_s,	// Optional second AXI4-Stream input.
	packed_t args		// Configuration parameters, see definitions.
	)
{
    #pragma HLS interface axis port=in1_s
    #pragma HLS interface axis port=in2_s
	#pragma HLS interface s_axilite port=args
	#pragma HLS interface s_axilite port=return

	channel_t in1_c, in2_c;	// To temporarily hold side-channel information.

	static int res_p[PIPE_DEPTH]; // Contains the partial results.
	unsigned int p = 0; // Current index within the partial results.
	unsigned int n = 0; // Indicates the n-th input value.

	// This loop calculates the partial results.

	#pragma HLS allocation function instances=do_operation limit=2
	#pragma HLS dependence variable=res_p inter false

	main: do {
		#pragma HLS pipeline II=1

		// Read the input stream.

		in1_c = in1_s.read();
		in1.u4 = in1_c.data;

		// Optionally read the second input stream.

		if(is_binary[args.element_op]) {
			in2_c = in2_s.read();
			in2.u4 = in2_c.data;
		}

		// Apply both the element-wise operation and the reduction operation
		// and store the first partial result.

		do_operation(&args.element_op, &in1.i4, &in2.i4);
		do_operation(&args.reduce_op, &res_p[p], &in1.i4);

		if(n++ < PIPE_DEPTH) res_p[p] = in1.i4;

		p = (p == PIPE_DEPTH - 1 ? 0 : p + 1);
	} while(in1_c.last != 1);

	// This loop reduces the partial results using a tree-like access pattern.

	#pragma HLS array_partition variable=res_p complete

	fini: for(int depth = 0; depth < LOG2_PIPE_DEPTH; depth++) {
		#pragma HLS unroll
		for(int i = 0; i < PIPE_DEPTH; i += (2 << depth)) {
			do_operation(&args.reduce_op, &res_p[i], &res_p[i + (1 << depth)]);
		}
	}

	do_operation(&args.final_op, &res_p[0], &res_p[1]);
	return res_p[0];
}

////////////////////////////////////////////////////////////////////////////////
