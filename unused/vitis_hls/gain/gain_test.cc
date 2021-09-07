/**
 *  @file    gain_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'gain.cc'.
 *
 */

#include <stdio.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// Side channels are TID, TUSR, TKEEP, TLAST.
// Helps interfacing with other cores e.g. TLAST for DMA.
typedef ap_axis<32, 1, 1, 1> AXI_T;
typedef hls::stream<AXI_T> STREAM_T;

void gain(STREAM_T &inStream, STREAM_T & outStream, int value);

int main() {
	STREAM_T inStream, outStream;

	for (int i = 0; i < 1000; i++) {
		AXI_T in;
		in.data = i;
		in.keep = 1;
		in.strb = 1;
		in.user = 1;
		in.id = 0;
		in.dest = 0;
		inStream << in;
	}

	gain(inStream, outStream, 5);

	for (int i = 0; i < 1000; i++) {
		AXI_T out;

		outStream >> out;
		printf("gain(%d, outStream, 5) = %d\n", i, static_cast<int>(out.data));
	}

	return 0;
}
