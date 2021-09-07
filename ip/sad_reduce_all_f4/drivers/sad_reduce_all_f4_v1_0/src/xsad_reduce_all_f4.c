// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xsad_reduce_all_f4.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XSad_reduce_all_f4_CfgInitialize(XSad_reduce_all_f4 *InstancePtr, XSad_reduce_all_f4_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XSad_reduce_all_f4_Start(XSad_reduce_all_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL) & 0x80;
    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XSad_reduce_all_f4_IsDone(XSad_reduce_all_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XSad_reduce_all_f4_IsIdle(XSad_reduce_all_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XSad_reduce_all_f4_IsReady(XSad_reduce_all_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XSad_reduce_all_f4_EnableAutoRestart(XSad_reduce_all_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XSad_reduce_all_f4_DisableAutoRestart(XSad_reduce_all_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_CTRL, 0);
}

u32 XSad_reduce_all_f4_Get_return(XSad_reduce_all_f4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_AP_RETURN);
    return Data;
}
void XSad_reduce_all_f4_InterruptGlobalEnable(XSad_reduce_all_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_GIE, 1);
}

void XSad_reduce_all_f4_InterruptGlobalDisable(XSad_reduce_all_f4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_GIE, 0);
}

void XSad_reduce_all_f4_InterruptEnable(XSad_reduce_all_f4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_IER);
    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_IER, Register | Mask);
}

void XSad_reduce_all_f4_InterruptDisable(XSad_reduce_all_f4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_IER);
    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_IER, Register & (~Mask));
}

void XSad_reduce_all_f4_InterruptClear(XSad_reduce_all_f4 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSad_reduce_all_f4_WriteReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_ISR, Mask);
}

u32 XSad_reduce_all_f4_InterruptGetEnabled(XSad_reduce_all_f4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_IER);
}

u32 XSad_reduce_all_f4_InterruptGetStatus(XSad_reduce_all_f4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSad_reduce_all_f4_ReadReg(InstancePtr->Control_BaseAddress, XSAD_REDUCE_ALL_F4_CONTROL_ADDR_ISR);
}

