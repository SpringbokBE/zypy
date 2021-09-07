// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xsad_reduce_all_f4.h"

extern XSad_reduce_all_f4_Config XSad_reduce_all_f4_ConfigTable[];

XSad_reduce_all_f4_Config *XSad_reduce_all_f4_LookupConfig(u16 DeviceId) {
	XSad_reduce_all_f4_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XSAD_REDUCE_ALL_F4_NUM_INSTANCES; Index++) {
		if (XSad_reduce_all_f4_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XSad_reduce_all_f4_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XSad_reduce_all_f4_Initialize(XSad_reduce_all_f4 *InstancePtr, u16 DeviceId) {
	XSad_reduce_all_f4_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XSad_reduce_all_f4_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XSad_reduce_all_f4_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

