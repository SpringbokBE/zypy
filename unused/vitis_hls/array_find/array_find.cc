/**
 *  @file    array_find.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that finds the given value in an array of characters.
 *
 */

void vector_find(char val, char in_vec[10], char out_vec[10]) {
	#pragma HLS INTERFACE ap_fifo port=in_vec
	#pragma HLS INTERFACE ap_fifo port=out_vec

	for (int i = 0; i < 10; i++) {
		if (in_vec[i] == val)
			out_vec[i] = 1;
		else
			out_vec[i] = 0;
	}
}
