// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XUFUNC_REDUCE_ALL_I4_H
#define XUFUNC_REDUCE_ALL_I4_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xufunc_reduce_all_i4_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_BaseAddress;
} XUfunc_reduce_all_i4_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XUfunc_reduce_all_i4;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XUfunc_reduce_all_i4_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XUfunc_reduce_all_i4_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XUfunc_reduce_all_i4_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XUfunc_reduce_all_i4_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XUfunc_reduce_all_i4_Initialize(XUfunc_reduce_all_i4 *InstancePtr, u16 DeviceId);
XUfunc_reduce_all_i4_Config* XUfunc_reduce_all_i4_LookupConfig(u16 DeviceId);
int XUfunc_reduce_all_i4_CfgInitialize(XUfunc_reduce_all_i4 *InstancePtr, XUfunc_reduce_all_i4_Config *ConfigPtr);
#else
int XUfunc_reduce_all_i4_Initialize(XUfunc_reduce_all_i4 *InstancePtr, const char* InstanceName);
int XUfunc_reduce_all_i4_Release(XUfunc_reduce_all_i4 *InstancePtr);
#endif

void XUfunc_reduce_all_i4_Start(XUfunc_reduce_all_i4 *InstancePtr);
u32 XUfunc_reduce_all_i4_IsDone(XUfunc_reduce_all_i4 *InstancePtr);
u32 XUfunc_reduce_all_i4_IsIdle(XUfunc_reduce_all_i4 *InstancePtr);
u32 XUfunc_reduce_all_i4_IsReady(XUfunc_reduce_all_i4 *InstancePtr);
void XUfunc_reduce_all_i4_EnableAutoRestart(XUfunc_reduce_all_i4 *InstancePtr);
void XUfunc_reduce_all_i4_DisableAutoRestart(XUfunc_reduce_all_i4 *InstancePtr);
u32 XUfunc_reduce_all_i4_Get_return(XUfunc_reduce_all_i4 *InstancePtr);

void XUfunc_reduce_all_i4_Set_args(XUfunc_reduce_all_i4 *InstancePtr, u32 Data);
u32 XUfunc_reduce_all_i4_Get_args(XUfunc_reduce_all_i4 *InstancePtr);

void XUfunc_reduce_all_i4_InterruptGlobalEnable(XUfunc_reduce_all_i4 *InstancePtr);
void XUfunc_reduce_all_i4_InterruptGlobalDisable(XUfunc_reduce_all_i4 *InstancePtr);
void XUfunc_reduce_all_i4_InterruptEnable(XUfunc_reduce_all_i4 *InstancePtr, u32 Mask);
void XUfunc_reduce_all_i4_InterruptDisable(XUfunc_reduce_all_i4 *InstancePtr, u32 Mask);
void XUfunc_reduce_all_i4_InterruptClear(XUfunc_reduce_all_i4 *InstancePtr, u32 Mask);
u32 XUfunc_reduce_all_i4_InterruptGetEnabled(XUfunc_reduce_all_i4 *InstancePtr);
u32 XUfunc_reduce_all_i4_InterruptGetStatus(XUfunc_reduce_all_i4 *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
