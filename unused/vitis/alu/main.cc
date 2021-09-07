/**
 *  @file    main.cc
 *  @author  Gerbrand De Laender
 *  @date    02/01/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, software testing
 *
 *  @section DESCRIPTION
 *
 *  Uses IP that makes use of a simple ALU for operations on integers.
 *
 */

#include <stdio.h>
#include <xalu.h>
#include <xalu_hw.h>

#include "platform.h"

volatile unsigned int * regA = reinterpret_cast<unsigned int *>(
	XPAR_ALU_0_S_AXI_CRTL_BUS_BASEADDR + XALU_CRTL_BUS_ADDR_A_DATA);
volatile unsigned int * regB = reinterpret_cast<unsigned int *>(
	XPAR_ALU_0_S_AXI_CRTL_BUS_BASEADDR + XALU_CRTL_BUS_ADDR_B_DATA);
volatile unsigned int * regOp = reinterpret_cast<unsigned int *>(
	XPAR_ALU_0_S_AXI_CRTL_BUS_BASEADDR + XALU_CRTL_BUS_ADDR_OP_DATA);
volatile unsigned int * regC = reinterpret_cast<unsigned int *>(
	XPAR_ALU_0_S_AXI_CRTL_BUS_BASEADDR + XALU_CRTL_BUS_ADDR_C_DATA);
volatile unsigned int * regCrtl = reinterpret_cast<unsigned int *>(
	XPAR_ALU_0_S_AXI_CRTL_BUS_BASEADDR);

void start_alu() {
	*regCrtl = (*regCrtl & 0x80) | 0x01;
}

unsigned int is_done_alu() {
	return (*regCrtl >> 1) & 0x01;
}

int main() {
	init_platform();

	printf("Using bare metal driver:\n---\n");

	*regA = 2; *regB = 3;
	*regOp = 0;
	start_alu();
	while(!is_done_alu());
	printf("%d + %d = %d\n", *regA, *regB, *regC);

	*regA = 7; *regB = 5;
	*regOp = 1;
	start_alu();
	while(!is_done_alu());
	printf("%d - %d = %d\n", *regA, *regB, *regC);

	*regA = 10; *regB = 2;
	*regOp = 2;
	start_alu();
	while(!is_done_alu());
	printf("%d * %d = %d\n", *regA, *regB, *regC);

	*regA = 50; *regB = 5;
	*regOp = 3;
	start_alu();
	while(!is_done_alu());
	printf("%d / %d = %d\n", *regA, *regB, *regC);

	cleanup_platform();
	return 0;
}
