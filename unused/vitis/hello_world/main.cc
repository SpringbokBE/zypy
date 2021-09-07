/**
 *  @file    main.cc
 *  @author  Gerbrand De Laender
 *  @date    18/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 *  Hello world in Vitis.
 *
 */

#include <stdio.h>
#include <xparameters.h>

#include "platform.h"

int main() {
	init_platform();

	printf("Hello, world!");

	cleanup_platform();
	return 0;
}
