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
 *  Uses IP to calculate some more complex function and which makes use of the
 *  BRAM interface to allow for fast storage of the results.
 *
 */
#include <stdio.h>
#include <xfunc_xy.h>
#include <math.h>
#include <xparameters.h>
#include <xtime_l.h>

#include "platform.h"

u32 float_to_u32(float val) {
	union {
		float f;
		u32 u;
	} data = { val };

	return data.u;
}

float u32_to_float(u32 val) {
	union {
		u32 u;
		float f;
	} data = { val };

	return data.f;
}

void init_func_xy(XFunc_xy *ip, XFunc_xy_Config *ip_cfg){
	ip_cfg = XFunc_xy_LookupConfig(XPAR_FUNC_XY_0_DEVICE_ID);

	if(ip_cfg){
		int status = XFunc_xy_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'func_xy'!\n");
	}
}

void func_xy(float xs[100], float y, float res[100]){
	for(int i = 0; i < 100; i++){
		res[i] = (sinf(xs[i]) + sqrtf(expf(y))) / 2;
	}
}

int main() {
	init_platform();

	// Initialize IP cores.
	XFunc_xy f;
	XFunc_xy_Config f_cfg;
	init_func_xy(&f, &f_cfg);

	XTime t1, t2;

	// Initialize structures.
	float xs_sw[100], res_sw[100];
	float *xs_hw = reinterpret_cast<float *>(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR);
	float *res_hw = reinterpret_cast<float *>(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR);

	for(int i = 0; i< 100; i++){
		xs_sw[i] = i;
		xs_hw[i] = i;
	}

	// Start test.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nSoftware implementation:\n---\n");
	XTime_GetTime(&t1);
	func_xy(xs_sw, 0.01f, res_sw);
	XTime_GetTime(&t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nHardware implementation:\n---\n");
	XTime_GetTime(&t1);
	XFunc_xy_Set_y(&f, float_to_u32(0.01f));
	XFunc_xy_Start(&f);
	while(XFunc_xy_IsDone(&f));
	XTime_GetTime(&t2);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	double error = 0;
	for(int i = 0; i < 100; i++)
		error += fabsf(res_hw[i] - res_sw[i]);
	printf("\nTotal average error = %f%%\n", error / 100.0f);

	return 0;
}
