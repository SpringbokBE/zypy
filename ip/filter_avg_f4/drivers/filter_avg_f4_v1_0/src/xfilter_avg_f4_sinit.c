// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xfilter_avg_f4.h"

extern XFilter_avg_f4_Config XFilter_avg_f4_ConfigTable[];

XFilter_avg_f4_Config *XFilter_avg_f4_LookupConfig(u16 DeviceId) {
	XFilter_avg_f4_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XFILTER_AVG_F4_NUM_INSTANCES; Index++) {
		if (XFilter_avg_f4_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XFilter_avg_f4_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XFilter_avg_f4_Initialize(XFilter_avg_f4 *InstancePtr, u16 DeviceId) {
	XFilter_avg_f4_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XFilter_avg_f4_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XFilter_avg_f4_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

