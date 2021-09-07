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
 *  Uses IP that simply multiplies two short unsigned integers.
 *
 */

#include <stdio.h>
#include <xparameters.h>
#include <xtime_l.h>

#include "platform.h"

unsigned int *multiplierAddr =
		reinterpret_cast<unsigned int *>(XPAR_MULTIPLIER_0_S00_AXI_BASEADDR);

#define multiplierRead() *(multiplierAddr + 1)
#define multiplierWrite(x) *(multiplierAddr + 0) = x

int main() {
	init_platform();

	short a = 2;
	short b = 33;
	int c, d;
	XTime t1, t2;

	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n",
			1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nSoftware implementation:\n---\n");
	XTime_GetTime(&t1);
	c = a * b;
	XTime_GetTime(&t2);
	printf("%d + %d = %d\n", a, b, c);
	printf("Took %f us (%llu ticks).\n",
			1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nHardware implementation:\n---\n");
	XTime_GetTime(&t1);
	multiplierWrite((a << 16) + b);
	d = multiplierRead();
	XTime_GetTime(&t2);
	printf("%d + %d = %d\n", a, b, d);
	printf("Took %f us (%llu ticks).\n",
			1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	cleanup_platform();
	return 0;
}
