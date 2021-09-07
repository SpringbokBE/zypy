// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1 ns / 1 ps
module ufunc_reduce_all_i4_exp_32_32_s_exp_x_msb_1_table_V_rom (
addr0, ce0, q0, clk);

parameter DWIDTH = 68;
parameter AWIDTH = 8;
parameter MEM_SIZE = 256;

input[AWIDTH-1:0] addr0;
input ce0;
output reg[DWIDTH-1:0] q0;
input clk;

reg [DWIDTH-1:0] ram[0:MEM_SIZE-1];

initial begin
    $readmemh("./ufunc_reduce_all_i4_exp_32_32_s_exp_x_msb_1_table_V_rom.dat", ram);
end



always @(posedge clk)  
begin 
    if (ce0) 
    begin
        q0 <= ram[addr0];
    end
end



endmodule

`timescale 1 ns / 1 ps
module ufunc_reduce_all_i4_exp_32_32_s_exp_x_msb_1_table_V(
    reset,
    clk,
    address0,
    ce0,
    q0);

parameter DataWidth = 32'd68;
parameter AddressRange = 32'd256;
parameter AddressWidth = 32'd8;
input reset;
input clk;
input[AddressWidth - 1:0] address0;
input ce0;
output[DataWidth - 1:0] q0;



ufunc_reduce_all_i4_exp_32_32_s_exp_x_msb_1_table_V_rom ufunc_reduce_all_i4_exp_32_32_s_exp_x_msb_1_table_V_rom_U(
    .clk( clk ),
    .addr0( address0 ),
    .ce0( ce0 ),
    .q0( q0 ));

endmodule

