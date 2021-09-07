/**
 * Test bench for filter_avg_f4.cc.
 */

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "filter_avg_f4.h"

////////////////////////////////////////////////////////////////////////////////

#define ERROR_TOLERANCE 0.0001
#define TEST_HEIGHT 100
#define TEST_WIDTH 100

////////////////////////////////////////////////////////////////////////////////

// Fill the given stream with random data.

void fill_stream(stream_t &s, float a[TEST_HEIGHT][TEST_WIDTH]=nullptr)
{
	channel_t c;
	for(int y = 0; y < TEST_HEIGHT; y++)
		for(int x = 0; x < TEST_WIDTH; x++) {
			in.f4 = (float) rand() / (float) RAND_MAX;
			if(a) a[y][x] = in.f4;
			c.data = in.u4;
			c.keep = 0xf;
			c.last = (y == TEST_HEIGHT - 1 and x == TEST_WIDTH - 1) ? 1 : 0;
			s.write(c);
		}
}

////////////////////////////////////////////////////////////////////////////////

void read_stream(stream_t &s, float a[TEST_HEIGHT][TEST_WIDTH])
{
	channel_t c;
	for(int y = 0; y < TEST_HEIGHT; y++)
		for(int x = 0; x < TEST_WIDTH; x++) {
			c = s.read();
			out.u4 = c.data;
			a[y][x] = out.f4;
		}
}

////////////////////////////////////////////////////////////////////////////////

// Print out a matrix in a human-readable format.

void print_mat(float mat[TEST_HEIGHT][TEST_WIDTH], const char* name)
{
	printf("%s\n---\n");
	for (int y = 0; y < TEST_HEIGHT; y++) {
		for (int x = 0; x < TEST_WIDTH; x++)
			printf ("%5.2f  ", mat[y][x]);
		printf("\n");
	}
}

////////////////////////////////////////////////////////////////////////////////

int test_filter_avg()
{
	printf("Starting 'filter_avg' test...\n");

	float in_a[TEST_HEIGHT][TEST_WIDTH], out_a[TEST_HEIGHT][TEST_WIDTH];

	stream_t in_s, out_s;
	fill_stream(in_s, in_a);

	// Run hardware implementation.

	printf("Running hardware implementation...\n");

	packed_t args = {
		.height = TEST_HEIGHT,
		.width = TEST_WIDTH
	};

	filter_avg_f4(in_s, out_s, args);
	read_stream(out_s, out_a);

	print_mat(in_a, "in_a");
	print_mat(out_a, "out_a");

	// Check if reference and hardware implementation match.

	for(int y = 1; y < TEST_HEIGHT - 1; y++)
		for(int x = 1; x < TEST_WIDTH - 1; x++) {
			float res = 0.2f * (in_a[y - 1][x] + in_a[y][x - 1]+ in_a[y][x]+ in_a[y][x + 1] + in_a[y + 1][x]);
			if(abs(out_a[y][x] - res) > ERROR_TOLERANCE)
					return -1;
		}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////

int main()
{
	bool error = false;

	error |= test_filter_avg();

	return error;
}

////////////////////////////////////////////////////////////////////////////////
