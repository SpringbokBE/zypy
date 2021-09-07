/**
 *  @file    array_find_test.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  Test bench for 'array_find.cc'.
 *
 */

#include <stdio.h>

void vector_find(char val, char in_vec[10], char out_vec[10]);

int main() {

	char in_vec[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1 };
	char out_vec[10];

	vector_find(1, in_vec, out_vec);

	for (int i = 0; i < 10; i++) {
		printf("out_vec[%d] = %d\n", i, out_vec[i]);
	}

	return 0;
}
