// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xfilter_avg_f4.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XFilter_avg_f4_CfgInitialize(XFilter_avg_f4 *InstancePtr, XFilter_avg_f4_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XFilter_avg_f4_Start(XFilter_avg_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL) & 0x80;
    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XFilter_avg_f4_IsDone(XFilter_avg_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XFilter_avg_f4_IsIdle(XFilter_avg_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XFilter_avg_f4_IsReady(XFilter_avg_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XFilter_avg_f4_EnableAutoRestart(XFilter_avg_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XFilter_avg_f4_DisableAutoRestart(XFilter_avg_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_AP_CTRL, 0);
}

void XFilter_avg_f4_Set_args(XFilter_avg_f4 *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_ARGS_DATA, Data);
}

u32 XFilter_avg_f4_Get_args(XFilter_avg_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_ARGS_DATA);
    return Data;
}

void XFilter_avg_f4_InterruptGlobalEnable(XFilter_avg_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_GIE, 1);
}

void XFilter_avg_f4_InterruptGlobalDisable(XFilter_avg_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_GIE, 0);
}

void XFilter_avg_f4_InterruptEnable(XFilter_avg_f4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_IER);
    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_IER, Register | Mask);
}

void XFilter_avg_f4_InterruptDisable(XFilter_avg_f4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_IER);
    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_IER, Register & (~Mask));
}

void XFilter_avg_f4_InterruptClear(XFilter_avg_f4 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFilter_avg_f4_WriteReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_ISR, Mask);
}

u32 XFilter_avg_f4_InterruptGetEnabled(XFilter_avg_f4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_IER);
}

u32 XFilter_avg_f4_InterruptGetStatus(XFilter_avg_f4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFilter_avg_f4_ReadReg(InstancePtr->Control_BaseAddress, XFILTER_AVG_F4_CONTROL_ADDR_ISR);
}

