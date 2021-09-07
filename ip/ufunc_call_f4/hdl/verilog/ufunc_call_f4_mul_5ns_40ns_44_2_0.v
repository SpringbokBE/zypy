// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================

`timescale 1 ns / 1 ps

module ufunc_call_f4_mul_5ns_40ns_44_2_0_Multiplier_2(clk, ce, a, b, p);
input clk;
input ce;
input [5 - 1 : 0] a;
input [40 - 1 : 0] b;
output[44 - 1 : 0] p;
reg signed [44 - 1 : 0] p;
wire [44 - 1 : 0] tmp_product;

assign tmp_product = a * b;
always @ (posedge clk) begin
    if (ce) begin
        p <= tmp_product;
    end
end
endmodule
`timescale 1 ns / 1 ps
module ufunc_call_f4_mul_5ns_40ns_44_2_0(
    clk,
    reset,
    ce,
    din0,
    din1,
    dout);

parameter ID = 32'd1;
parameter NUM_STAGE = 32'd1;
parameter din0_WIDTH = 32'd1;
parameter din1_WIDTH = 32'd1;
parameter dout_WIDTH = 32'd1;
input clk;
input reset;
input ce;
input[din0_WIDTH - 1:0] din0;
input[din1_WIDTH - 1:0] din1;
output[dout_WIDTH - 1:0] dout;



ufunc_call_f4_mul_5ns_40ns_44_2_0_Multiplier_2 ufunc_call_f4_mul_5ns_40ns_44_2_0_Multiplier_2_U(
    .clk( clk ),
    .ce( ce ),
    .a( din0 ),
    .b( din1 ),
    .p( dout ));

endmodule

