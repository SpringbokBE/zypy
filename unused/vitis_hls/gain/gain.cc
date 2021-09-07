/**
 *  @file    gain.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that applies gain to an incoming AXI stream.
 *
 */

#include <hls_stream.h>
#include <ap_axi_sdata.h>

// Side channels are TID, TUSR, TKEEP, TLAST.
// Helps interfacing with other cores e.g. TLAST for DMA.
typedef ap_axis<32, 1, 1, 1> AXI_T;
typedef hls::stream<AXI_T> STREAM_T;

void gain(STREAM_T &inStream, STREAM_T & outStream, int value) {
	#pragma HLS INTERFACE axis port=inStream
	#pragma HLS INTERFACE axis port=outStream
	#pragma HLS INTERFACE s_axilite port=value bundle=CRTL_BUS
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	for (int i = 0; i < 1000; i++) {
		#pragma HLS PIPELINE
		AXI_T in, out;

		// Block if FIFO sender is empty.
		inStream >> in;

		out = in;
		out.data = in.data * value;

		// Block if FIFO receiver is full.
		outStream << out;
	}
}
