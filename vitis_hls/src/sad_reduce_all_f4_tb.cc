/**
 * Test bench for sad_reduce_all_f4.cc.
 */

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <cmath>

#include "sad_reduce_all_f4.h"

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
	
	float out = sad_reduce_all_f4(in1_s, in2_s);
	
	printf("out=%.3f\n", out);

	// Check if reference and hardware implementation match.

	return (abs(out - out_ref) > ERROR_TOLERANCE);
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
	bool error = false;

	error |= test_sad();

	return error;
}

////////////////////////////////////////////////////////////////////////////////
