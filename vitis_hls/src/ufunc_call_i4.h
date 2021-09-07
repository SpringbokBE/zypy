/**
 * Header file for ufunc_reduce_all_i4.cc.
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>

////////////////////////////////////////////////////////////////////////////////

// Custom type to hold a int along with AXI-Stream side-channel information.

typedef ap_axiu<8 * sizeof(int), 1, 1, 1> channel_t;

// Custom type to hold an AXI-Stream.

typedef hls::stream<channel_t> stream_t;

// Custom type to pack AXI4-Lite parameters together.

typedef struct {
	unsigned char element_op; 	// Element-wise operation to use.
} packed_t;

// Union used to convert between int (processing) and unsigned integer (stream).
// See https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/Transfer-various-data-types-through-a-single-axi4-stream/td-p/925659

union {
	unsigned int u4;
	int i4;
} in1, in2, out1, out2;

////////////////////////////////////////////////////////////////////////////////

void ufunc_call_i4(
	stream_t &in1_s,		// Main AXI4-Stream input.
	stream_t &in2_s,		// Optional second AXI4-Stream input.
	stream_t &out1_s,		// Main AXI4-Stream output.
	stream_t &out2_s,		// Optional second AXI4-Stream output.
	packed_t args			// Ufunc configuration parameters, see definition.
	);

////////////////////////////////////////////////////////////////////////////////
