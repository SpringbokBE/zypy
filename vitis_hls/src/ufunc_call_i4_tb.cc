/**
 * Test bench for ufunc_call_i4.cc.
 */

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <cmath>

#include "ufunc_call_i4.h"
#include "ufuncs_i4.h"

////////////////////////////////////////////////////////////////////////////////

#define LEN 1000
#define ERROR_TOLERANCE 0.0001

////////////////////////////////////////////////////////////////////////////////

void fill_stream(stream_t &s, int *a=nullptr, int len=0, int value=0)
{
	srand(123);
	channel_t c;

	for(int i = 0; i < len; i++){
		if(value)
			in1.i4 = a[i] = value;
		else
			in1.i4 = a[i] = (int) rand();

		c.data = in1.u4;
		c.keep = 0xf;
		c.last = (i == len - 1) ? 1 : 0;
		s.write(c);
	}
}

////////////////////////////////////////////////////////////////////////////////

void read_stream(stream_t &s, int *a=nullptr, int len=0)
{
    if(not a) return;

	channel_t c;

	for(int i = 0; i < len; i++){
		c = s.read();
		out1.u4 = c.data;
		a[i] = out1.i4;
        if(c.last) break;
	}
}

////////////////////////////////////////////////////////////////////////////////

int test_add_call()
{
	printf("Starting 'add_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = in1_a[i] + in2_a[i];

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = ADD
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_subtract_call()
{
	printf("Starting 'subtract_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = in1_a[i] - in2_a[i];

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = SUBTRACT
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_multiply_call()
{
	printf("Starting 'multiply_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = in1_a[i] * in2_a[i];

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = MULTIPLY
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_true_divide_call()
{
	printf("Starting 'true_divide_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = in1_a[i] / in2_a[i];

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = TRUE_DIVIDE
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_sin_call()
{
	printf("Starting 'sin_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = (int) (sin(in1_a[i]) + 0.5f);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = SIN
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_sqrt_call()
{
	printf("Starting 'sqrt_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = (int) (sqrt(in1_a[i]) + 0.5f);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = SQRT
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_arctan_call()
{
	printf("Starting 'arctan_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = (int) (atan(in1_a[i]) + 0.5f);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = ARCTAN,
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_sinh_call()
{
	printf("Starting 'sinh_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = (int) (sinh(in1_a[i]) + 0.5f);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = SINH
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_exp_call()
{
	printf("Starting 'exp_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN, 5);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = (int) (exp(in1_a[i]) + 0.5f);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = EXP
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int test_log_call()
{
	printf("Starting 'log_call' test...\n");

	int in1_a[LEN], in2_a[LEN], out1_a[LEN], out1_a_ref[LEN];
	stream_t in1_s, in2_s, out1_s, out2_s;

	fill_stream(in1_s, in1_a, LEN);
	fill_stream(in2_s, in2_a, LEN);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	for(int i = 0; i < LEN; i++)
		out1_a_ref[i] = (int) (log(in1_a[i]) + 0.5f);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.element_op = LOG
	};

	ufunc_call_i4(in1_s, in2_s, out1_s, out2_s, args);

    read_stream(out1_s, out1_a, LEN);

	// Check if reference and hardware implementation match.

	for(int i = 0; i < LEN; i++)
        if(abs(out1_a[i] - out1_a_ref[i]) > ERROR_TOLERANCE)
            return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
	bool error = false;

	error |= test_add_call();
	error |= test_subtract_call();
	error |= test_multiply_call();
	error |= test_true_divide_call();
	error |= test_sqrt_call();
	error |= test_exp_call();
	error |= test_log_call();

	return error;
}

////////////////////////////////////////////////////////////////////////////////
