/**
 * IP implementing the __call__ method of NumPy ufuncs for one-dimensional
 * inputs (or flattened multi-dimensional inputs). Integer (i4) implementation.
 *
 * The operator can selected using the `args.element_op` parameter.
 *
 * IMPORTANT:
 *  - If there are two input streams, the args.element_op needs to be a binary
 *    operation.
 *
 * EXAMPLES:
 *	- To calculate the the sin of each element of a single input stream:
 *	    > args.element_op = SIN
 *	- To calculate the product of two input streams:
 *	    > args.element_op = MULTIPLY
 */

////////////////////////////////////////////////////////////////////////////////

#include "ufunc_call_i4.h"
#include "ufuncs_i4.h"

////////////////////////////////////////////////////////////////////////////////

// Function that selects and performs the required element-wise operation on
// inputs x and y. The result(s) is/are stored back in x (and y).

void do_operation(unsigned char *op, int *x, int *y)
{
	#pragma HLS inline

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
		case SUBTRACT:
			ufunc_out = subtract(ufunc_in);
			break;
		case MULTIPLY:
			ufunc_out = multiply(ufunc_in);
			break;
		case TRUE_DIVIDE:
			ufunc_out = true_divide(ufunc_in);
			break;
		case SQRT:
			ufunc_out = sqrt(ufunc_in);
			break;
		case EXP:
			ufunc_out = exp(ufunc_in);
			break;
		case LOG:
			ufunc_out = log(ufunc_in);
			break;
	}

	*x = ufunc_out.first;
	*y = ufunc_out.second;
}

////////////////////////////////////////////////////////////////////////////////

// Top level module implementing the __call__ method of NumPy ufuncs for
// one-dimensional inputs (or flattened multi-dimensional inputs).

void ufunc_call_i4(
	stream_t &in1_s,		// Main AXI4-Stream input.
	stream_t &in2_s,		// Optional second AXI4-Stream input.
	stream_t &out1_s,		// Main AXI4-Stream output.
	stream_t &out2_s,		// Optional second AXI4-Stream output.
	packed_t args			// Ufunc configuration parameters, see definition.
	)
{
    #pragma HLS interface axis port=in1_s
	#pragma HLS interface axis port=in2_s
	#pragma HLS interface axis port=out1_s
	#pragma HLS interface axis port=out2_s
	#pragma HLS interface s_axilite port=args
	#pragma HLS interface s_axilite port=return

	channel_t in1_c, in2_c,
              out1_c, out2_c;	// To temporarily hold side-channel information.

	// This loop calculates the results.

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

		// Apply the element-wise operation.

		do_operation(&args.element_op, &in1.i4, &in2.i4);

		// Write to the output stream.

		out1_c.data = in1.u4;
		out1_c.keep = in1_c.keep;
		out1_c.last = in1_c.last;
		out1_s.write(out1_c);

		// Optionally write to the second output stream.

		if(returns_pair[args.element_op]) {
			out2.i4 = in2.i4;
			out2_c.data = out2.u4;
			out2_c.keep = in1_c.keep;
			out2_c.last = in1_c.last;
			out2_s.write(out2_c);
		}
	} while(in1_c.last != 1);
}

////////////////////////////////////////////////////////////////////////////////
