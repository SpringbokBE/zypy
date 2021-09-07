/**
 * Header file for sad_reduce_all_f4.cc.
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>

////////////////////////////////////////////////////////////////////////////////

// Specify the (ceil of the) log2 of the pipeline depth below.
// Not adapting this value correctly will result in co-simulation failure.
//
// The number of partial results that are required depends on the depth of the
// "main" pipeline. Adding an operation with a very long latency will therefore
// impact the number of partial results that have to be calculated.
//
// The depth of the main pipeline can be found in the synthesis report. If it is
// 13 for example, LOG2_PIPE_DEPTH should be set to ceil(log2(13)) = 4.

#define LOG2_PIPE_DEPTH 4
#define PIPE_DEPTH (1U << LOG2_PIPE_DEPTH)

////////////////////////////////////////////////////////////////////////////////

// Custom type to hold a float along with AXI-Stream side-channel information.

typedef ap_axiu<8 * sizeof(float), 1, 1, 1> channel_t;

// Custom type to hold an AXI-Stream.

typedef hls::stream<channel_t> stream_t;

// Union used to convert between float (processing) and unsigned integer (stream).
// See https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/Transfer-various-data-types-through-a-single-axi4-stream/td-p/925659

union {
	unsigned int u4;
	float f4;
} in1, in2;

////////////////////////////////////////////////////////////////////////////////

float sad_reduce_all_f4(
	stream_t &in1_s,	// Main AXI4-Stream input.
	stream_t &in2_s		// Second AXI4-Stream input.
	);

////////////////////////////////////////////////////////////////////////////////
