/**
 *  @file    test.h
 *  @author  Gerbrand De Laender
 *  @date    07/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Test bench helper functions. Template parameters are:
 *  T = Data type excluding AXI4-Stream side channels
 *
 */

#pragma once

#include <iostream>
#include <string>
#include <stdio.h>

#include "np_arctan2_f8.h"

// Print out an array in a human-readable format.
template <typename T>
void print_arr(T *arr, std::string name, int len)

{
	std::cout << name << "\n---\n";
	for (int i = 0; i < len; i++) {
		#if defined(TYPE_FLOAT) || defined(TYPE_DOUBLE)
			printf("%5.2f  ", arr[i]);
		#else
			printf("%d  ", arr[i]);
		#endif
	}
	printf("\n");
}


// Print out a matrix in a human-readable format.
template <typename T>
void print_mat(T *mat, std::string name, int rows, int cols)

{
	std::cout << name << "\n---\n";
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			#if defined(TYPE_FLOAT) || defined(TYPE_DOUBLE)
				printf ("%5.2f  ", mat[i * sizeof(T) + j]);
			#else
				printf ("%d  ", mat[i * sizeof(T) + j]);
			#endif
		}
		printf("\n");
	}
}
