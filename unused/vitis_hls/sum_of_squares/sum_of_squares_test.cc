/**
 *  @file    sum_of_squares_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'sum_of_squares.cc'.
 *
 */

#include <stdio.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define N 1024

// Side channels are TID, TUSR, TKEEP, TLAST.
// Helps interfacing with other cores e.g. TLAST for DMA.
typedef ap_axis <32,1,1,1> AXI_T;
typedef hls::stream<AXI_T> STREAM_T;

ap_int<32> sum_of_squares(STREAM_T &aStream);

int main(){
	STREAM_T aStream;

	for(int i = 0; i < N; i++){
		AXI_T in;
		in.data = i;
		in.keep = 1;
		in.strb = 1;
		in.user = 1;
		in.id = 0;
		in.dest = 0;
		aStream << in;
	}

	ap_int<32> res = sum_of_squares(aStream);
	printf("res = %d\n", static_cast<int>(res));

	return 0;
}
