/**
 * Header file for filter_avg_f4.cc.
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>

////////////////////////////////////////////////////////////////////////////////

#define AVG(w) 0.2f * (lb_1.read(w - 2) + lb_2.read() + lb_3.read() + lb_4.read() + new_value)
#define SHIFT_IN(x, w) lb_1.shift(lb_2.shift(lb_3.shift(lb_4.shift(lb_5.shift(x, w - 3)))), w - 2)

////////////////////////////////////////////////////////////////////////////////

// Custom type to hold a float along with AXI-Stream side-channel information.

typedef ap_axiu<8 * sizeof(float), 1, 1, 1> channel_t;

// Custom type to hold an AXI-Stream.

typedef hls::stream<channel_t> stream_t;

// Custom type to pack AXI4-Lite parameters together.

typedef struct {
	unsigned short height; 	// Height of the input matrix.
	unsigned short width; 	// Width of the input matrix.
} packed_t;

// Union used to convert between float (processing) and unsigned integer (stream).
// See https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/Transfer-various-data-types-through-a-single-axi4-stream/td-p/925659

union {
	unsigned int u4;
	float f4;
} in, out;

////////////////////////////////////////////////////////////////////////////////

void filter_avg_f4(
	stream_t &in_s,
	stream_t &out_s,
	packed_t args
	);

////////////////////////////////////////////////////////////////////////////////
