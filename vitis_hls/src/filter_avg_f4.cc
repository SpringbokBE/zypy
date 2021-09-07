/**
 * IP implementing an average filter. Floating point (f4) implementation.
 *
 * For each output coordinate that is not on the border, the average is
 * calculated as follows:
 *
 * out(x, y) =
 * in(x, y) + in(x, y - 1) + in(x, y + 1) + in(x - 1, y) + in(x + 1, y)
 *  
 * IMPLEMENTATION:
 *  - To minimise the I/O overhead, a line buffer is used to store the last
 *    "2 * WIDTH" values that have been read from the input stream. After
 *    filling the line buffer, the core reads one new value every cycle and
 *    produces one output every cycle. The buffer acts like a shift register.
 * 
 * IMPORTANT:
 *  - The maximum width of the matrix is limited to 2048 (1 << 11) because
 * 	  the capacity of the memories for the shift registers is limited to 1M bits.
 *    Furthermore, we can only use a limited amount of LUTs as memory (Vivado).
 * 	  Alternative is to implement the shift registers using BRAM (manually).
 * 
 */

////////////////////////////////////////////////////////////////////////////////

#include "filter_avg_f4.h"

#include "ap_shift_reg.h"

////////////////////////////////////////////////////////////////////////////////

#define MAX_WIDTH (1U << 11)

////////////////////////////////////////////////////////////////////////////////

// Read item from the stream and return its floating point value.

inline float read_stream(stream_t &stream)
{
	channel_t c;

	c = stream.read();
	in.u4 = c.data;

	return in.f4;
}

////////////////////////////////////////////////////////////////////////////////

// Write floating point value to the stream.

inline void write_stream(stream_t &stream, float value)
{
	channel_t c;
	
	out.f4 = value;
	c.data = out.u4;
	c.keep = 0xf;

	stream.write(c);
}

////////////////////////////////////////////////////////////////////////////////

// Write last floating point value to the stream.

inline void write_stream_last(stream_t &stream, float value)
{
	channel_t c;
	
	out.f4 = value;
	c.data = out.u4;
	c.keep = 0xf;
	c.last = 1;

	stream.write(c);
}

////////////////////////////////////////////////////////////////////////////////

// Check if the given x-coordinate is on the border or not.

inline bool on_border_x(int x, int width)
{
	return (x == 0 or x == width - 1);
}

////////////////////////////////////////////////////////////////////////////////

// Top level module implementing the filter functionality.

void filter_avg_f4(
	stream_t &in_s,
	stream_t &out_s,
	packed_t args
	)
{
	#pragma HLS interface axis port=in_s
	#pragma HLS interface axis port=out_s
	#pragma HLS interface s_axilite port=args
	#pragma HLS interface s_axilite port=return

	unsigned short lb_size = 2 * args.width;

	ap_shift_reg<float, MAX_WIDTH - 1> lb_1;
	ap_shift_reg<float, 1> lb_2;
	ap_shift_reg<float, 1> lb_3;
	ap_shift_reg<float, 1> lb_4;
	ap_shift_reg<float, MAX_WIDTH - 2> lb_5;

	float new_value;

	// The first element is unused when calculating the average over a cross.
	write_stream(out_s, read_stream(in_s));

	// Read in the complete line buffer and stream out the first row
	// and the first element of the second row.
	fill_lb: for(int i = 0; i < lb_size; i++) {
		new_value = read_stream(in_s);
	 	SHIFT_IN(new_value, args.width);
		if(i < args.width)
			write_stream(out_s, new_value);
	}

	// Start the regime.
	bool first = true;
	
	main_y: for(int y = 1; y < args.height - 1; y++) {
		main_x: for (int x = 0; x < args.width; x++) {
				// To make sure that first element of the second row is skipped.
				if(first) {
					first = false;
					continue;
				}

				// Read in new value and output the new result. If (y, x) is currently
				// on the border, output the border value instead of the average.
				new_value = read_stream(in_s);
				write_stream(out_s, on_border_x(x, args.width) ? lb_3.read() : AVG(args.width));

				// Shift the new element into the line buffer.
				SHIFT_IN(new_value, args.width);
		}
	}

	// Stream out the last remaining row, which is left untouched.
	 write_stream(out_s, lb_3.read());
	 write_stream(out_s, lb_4.read());

	empty_lb: for(int i = args.width - 3; i > 0; i--)
		write_stream(out_s, lb_5.read(i));

	write_stream_last(out_s, lb_5.read(0));
}

////////////////////////////////////////////////////////////////////////////////
