/**
 * Test bench for ufunc_reduce_all_f4.cc.
 */

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <cmath>

#include "ufunc_reduce_all_f4.h"
#include "ufuncs_f4.h"

////////////////////////////////////////////////////////////////////////////////

#define LEN 1000
#define ERROR_TOLERANCE 0.02

////////////////////////////////////////////////////////////////////////////////

void fill_stream(stream_t &s, float *a=nullptr, int len=0, float value=0.0f)
{
	srand(123);
	channel_t c;

	for(int i = 0; i < len; i++){
		if(value)
			in1.f4 = a[i] = value;
		else
			in1.f4 = a[i] = (float) rand() / (float) RAND_MAX;

		c.data = in1.u4;
		c.keep = 0xf;
		c.last = (i == len - 1) ? 1 : 0;
		s.write(c);
	}
}

////////////////////////////////////////////////////////////////////////////////

int test_add_reduce_all()
{
	printf("Starting 'add_reduce_all' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 5.0f);
	fill_stream(in2_s, in2_a, LEN, 5.0f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	
	float out_ref = in1_a[0];

	for(int i = 1; i < LEN; i++)
		out_ref += in1_a[i];

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = ADD,
		.element_op = NONE,
		.final_op = NONE
	};

	float out = ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

// The subtract reduction can be derived from the add reduction using:
// a - b - c - d - ... = a - (b + c + d + ...)

int test_subtract_reduce_all()
{
	printf("Starting 'subtract_reduce_all' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, &in1_a[1], LEN - 1, 5.0f);
	fill_stream(in2_s, &in2_a[1], LEN - 1, 5.0f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = in1_a[0];

	for(int i = 1; i < LEN; i++)
		out_ref -= in1_a[i];

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = ADD,
		.element_op = NONE,
		.final_op = NONE
	};

	float out = in1_a[0] - ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

int test_multiply_reduce_all()
{
	printf("Starting 'multiply_reduce_all' test...\n");
	
	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 1.00123f);
	fill_stream(in2_s, in2_a, LEN, 1.00123f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = in1_a[0];

	for(int i = 1; i < LEN; i++)
		out_ref *= in1_a[i];

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = MULTIPLY,
		.element_op = NONE,
		.final_op = NONE
	};
	
	float out = ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

// The true_divide reduction can be derived from the multiply reduction using:
// a / b / c / d / ... = a / (b * c * d * ...)

int test_true_divide_reduce_all()
{
	printf("Starting 'true_divide_reduce_all' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, &in1_a[1], LEN - 1, 1.00123f);
	fill_stream(in2_s, &in2_a[1], LEN - 1, 1.00123f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = in1_a[0];

	for(int i = 1; i < LEN; i++)
		out_ref /= in1_a[i];

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = MULTIPLY,
		.element_op = NONE,
		.final_op = NONE
	};

	float out = in1_a[0] / ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

// The logaddexp reduction can be derived from the sum of exponentials using:
// log(exp(log(exp(a) + exp(b)) + exp(c)) ... = log(exp(a) + exp(b) + exp(c) + ...)

int test_logaddexp_reduce_all()
{
	printf("Starting 'logaddexp_reduce_all' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 1.00123f);
	fill_stream(in2_s, in2_a, LEN, 1.00123f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = expf(in1_a[0]);

	for(int i = 1; i < LEN; i++)
		out_ref += expf(in1_a[i]);

	out_ref = logf(out_ref);

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = ADD,
		.element_op = EXP,
		.final_op = LOG
	};

	float out = ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

// The logaddexp2 reduction can be derived from the sum of exponentials using:
// log2(exp(log(exp(a) + exp(b)) + exp(c)) ... = log2(exp(a) + exp(b) + exp(c) + ...)

int test_logaddexp2_reduce_all()
{
	printf("Starting 'logaddexp2_reduce_all' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 1.00123f);
	fill_stream(in2_s, in2_a, LEN, 1.00123f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = expf(in1_a[0]);

	for(int i = 1; i < LEN; i++)
		out_ref += expf(in1_a[i]);

	out_ref = log2f(out_ref);

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = ADD,
		.element_op = EXP,
		.final_op = LOG
	};

	float out = ufunc_reduce_all_f4(in1_s, in2_s, args) / logf(2);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

int test_dot()
{
	printf("Starting 'dot' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 1.0f);
	fill_stream(in2_s, in2_a, LEN, 2.0f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = 0;

	for(int i = 0; i < LEN; i++)
		out_ref += (in1_a[i] * in2_a[i]);

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = ADD,
		.element_op = MULTIPLY,
		.final_op = NONE
	};

	float out = ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

int test_sum_of_squares()
{
	printf("Starting 'sum_of_squares' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 5.0f);
	fill_stream(in2_s, in2_a, LEN, 5.0f);

	// Run reference implementation.

	printf("Running reference implementation...\n");
	float out_ref = in1_a[0] * in1_a[0];

	for(int i = 1; i < LEN; i++)
		out_ref += in1_a[i] * in1_a[i];

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.reduce_op = ADD,
		.element_op = SQUARE,
		.final_op = NONE
	};

	float out = ufunc_reduce_all_f4(in1_s, in2_s, args);

	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

int test_sad()
{
	printf("Starting 'sad' test...\n");

	float in1_a[LEN], in2_a[LEN];
	stream_t in1_s, in2_s;

	fill_stream(in1_s, in1_a, LEN, 5.0f);
	fill_stream(in2_s, in2_a, LEN, 3.0f);

	// Run reference implementation.

	printf("Running reference implementation...\n");

	float out_ref = abs(in1_a[0] - in2_a[0]);

	for(int i = 1; i < LEN; i++)
		out_ref += abs(in1_a[i] - in2_a[i]);

	printf("out_ref=%.3f\n", out_ref);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");
	
	packed_t args = {
		.reduce_op = ADD,
		.element_op = ABSDIFF,
		.final_op = NONE
	};
	
	float out = ufunc_reduce_all_f4(in1_s, in2_s, args);
	
	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
	bool error = false;

	error |= test_add_reduce_all();
	error |= test_subtract_reduce_all();
	error |= test_multiply_reduce_all();
	error |= test_true_divide_reduce_all();
	error |= test_logaddexp_reduce_all();
	error |= test_logaddexp2_reduce_all();
	error |= test_dot();
	error |= test_sum_of_squares();
	error |= test_sad();

	return error;
}

////////////////////////////////////////////////////////////////////////////////
