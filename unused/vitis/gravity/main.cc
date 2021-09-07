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
 *  Make use of an IP that is able to calculate the gravitational force
 *  between two objects, given their masses and distance. Illustrates the
 *  usage of the AXI Lite interface.
 *
 */

#include <stdio.h>
#include <xgravity.h>
#include <xparameters.h>
#include <xtime_l.h>

#include "platform.h"

float reference[] = { 2000000000.0, 2000.0, 500.0, 222.22222222222223, 125.0,
		80.0, 55.55555555555556, 40.816326530612244, 31.25, 24.691358024691358 };

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

void init_gravity(XGravity *ip, XGravity_Config *ip_cfg) {
	ip_cfg = XGravity_LookupConfig(XPAR_XGRAVITY_0_DEVICE_ID);

	if (ip_cfg) {
		int status = XGravity_CfgInitialize(ip, ip_cfg);

		if (status != XST_SUCCESS)
			printf("Error initializing 'gravity'!\n");
	}
}

float gravity(float m1, float m2, float dist) {
	float dist_squared = dist ? dist * dist : 0.000001f;
	return 10.0f * (m1 * m2) / dist_squared;;
}

int main() {
	init_platform();

	// Initialize IP cores.
	XGravity g;
	XGravity_Config g_cfg;
	init_gravity(&g, &g_cfg);

	XTime t1, t2;

	// Start test.
	printf("\nTimer setup:\n---\n");
	XTime_GetTime(&t1);
	XTime_GetTime(&t2);
	printf("Empty run took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nSoftware implementation:\n---\n");
	int d;
	float force, error = 0.0;
	XTime_GetTime(&t1);
	for (d = 0; d < 10; d++) {
		force = gravity(10.0f, 20.0f, (float) d);
		error += force - reference[d];
	}
	XTime_GetTime(&t2);
	printf("gravity(10.0, 20.0, %f) = %f (accumulated error = %f)\n", (float) d, force, error);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	printf("\nHardware implementation:\n---\n");
	XTime_GetTime(&t1);
	for (d = 0; d < 10; d++) {
		XGravity_Set_m1(&g, float_to_u32(10.0f));
		XGravity_Set_m2(&g, float_to_u32(20.0f));
		XGravity_Set_d(&g, float_to_u32(d));
		XGravity_Start(&g);

		while (!XGravity_IsDone(&g))
			;

		force = u32_to_float(XGravity_Get_return(&g));
		error += force - reference[d];
	}
	XTime_GetTime(&t2);
	printf("gravity(10.0, 20.0, %f) = %f (accumulated error = %f)\n", (float) d, force, error);
	printf("Took %f us (%llu ticks).\n", 1000000.0 * (t2 - t1) / COUNTS_PER_SECOND, 2 * (t2 - t1));

	return 0;
}
