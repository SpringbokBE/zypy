// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XSAD_REDUCE_ALL_F4_H
#define XSAD_REDUCE_ALL_F4_H

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
#include "xsad_reduce_all_f4_hw.h"

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
} XSad_reduce_all_f4_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XSad_reduce_all_f4;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XSad_reduce_all_f4_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XSad_reduce_all_f4_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XSad_reduce_all_f4_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XSad_reduce_all_f4_ReadReg(BaseAddress, RegOffset) \
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
int XSad_reduce_all_f4_Initialize(XSad_reduce_all_f4 *InstancePtr, u16 DeviceId);
XSad_reduce_all_f4_Config* XSad_reduce_all_f4_LookupConfig(u16 DeviceId);
int XSad_reduce_all_f4_CfgInitialize(XSad_reduce_all_f4 *InstancePtr, XSad_reduce_all_f4_Config *ConfigPtr);
#else
int XSad_reduce_all_f4_Initialize(XSad_reduce_all_f4 *InstancePtr, const char* InstanceName);
int XSad_reduce_all_f4_Release(XSad_reduce_all_f4 *InstancePtr);
#endif

void XSad_reduce_all_f4_Start(XSad_reduce_all_f4 *InstancePtr);
u32 XSad_reduce_all_f4_IsDone(XSad_reduce_all_f4 *InstancePtr);
u32 XSad_reduce_all_f4_IsIdle(XSad_reduce_all_f4 *InstancePtr);
u32 XSad_reduce_all_f4_IsReady(XSad_reduce_all_f4 *InstancePtr);
void XSad_reduce_all_f4_EnableAutoRestart(XSad_reduce_all_f4 *InstancePtr);
void XSad_reduce_all_f4_DisableAutoRestart(XSad_reduce_all_f4 *InstancePtr);
u32 XSad_reduce_all_f4_Get_return(XSad_reduce_all_f4 *InstancePtr);


void XSad_reduce_all_f4_InterruptGlobalEnable(XSad_reduce_all_f4 *InstancePtr);
void XSad_reduce_all_f4_InterruptGlobalDisable(XSad_reduce_all_f4 *InstancePtr);
void XSad_reduce_all_f4_InterruptEnable(XSad_reduce_all_f4 *InstancePtr, u32 Mask);
void XSad_reduce_all_f4_InterruptDisable(XSad_reduce_all_f4 *InstancePtr, u32 Mask);
void XSad_reduce_all_f4_InterruptClear(XSad_reduce_all_f4 *InstancePtr, u32 Mask);
u32 XSad_reduce_all_f4_InterruptGetEnabled(XSad_reduce_all_f4 *InstancePtr);
u32 XSad_reduce_all_f4_InterruptGetStatus(XSad_reduce_all_f4 *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
