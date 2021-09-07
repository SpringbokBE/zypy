// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xufunc_reduce_all_i4.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XUfunc_reduce_all_i4_CfgInitialize(XUfunc_reduce_all_i4 *InstancePtr, XUfunc_reduce_all_i4_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XUfunc_reduce_all_i4_Start(XUfunc_reduce_all_i4 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL) & 0x80;
    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XUfunc_reduce_all_i4_IsDone(XUfunc_reduce_all_i4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XUfunc_reduce_all_i4_IsIdle(XUfunc_reduce_all_i4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XUfunc_reduce_all_i4_IsReady(XUfunc_reduce_all_i4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XUfunc_reduce_all_i4_EnableAutoRestart(XUfunc_reduce_all_i4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XUfunc_reduce_all_i4_DisableAutoRestart(XUfunc_reduce_all_i4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_CTRL, 0);
}

u32 XUfunc_reduce_all_i4_Get_return(XUfunc_reduce_all_i4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_AP_RETURN);
    return Data;
}
void XUfunc_reduce_all_i4_Set_args(XUfunc_reduce_all_i4 *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_ARGS_DATA, Data);
}

u32 XUfunc_reduce_all_i4_Get_args(XUfunc_reduce_all_i4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_ARGS_DATA);
    return Data;
}

void XUfunc_reduce_all_i4_InterruptGlobalEnable(XUfunc_reduce_all_i4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_GIE, 1);
}

void XUfunc_reduce_all_i4_InterruptGlobalDisable(XUfunc_reduce_all_i4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_GIE, 0);
}

void XUfunc_reduce_all_i4_InterruptEnable(XUfunc_reduce_all_i4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_IER);
    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_IER, Register | Mask);
}

void XUfunc_reduce_all_i4_InterruptDisable(XUfunc_reduce_all_i4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_IER);
    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_IER, Register & (~Mask));
}

void XUfunc_reduce_all_i4_InterruptClear(XUfunc_reduce_all_i4 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUfunc_reduce_all_i4_WriteReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_ISR, Mask);
}

u32 XUfunc_reduce_all_i4_InterruptGetEnabled(XUfunc_reduce_all_i4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_IER);
}

u32 XUfunc_reduce_all_i4_InterruptGetStatus(XUfunc_reduce_all_i4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XUfunc_reduce_all_i4_ReadReg(InstancePtr->Control_BaseAddress, XUFUNC_REDUCE_ALL_I4_CONTROL_ADDR_ISR);
}

