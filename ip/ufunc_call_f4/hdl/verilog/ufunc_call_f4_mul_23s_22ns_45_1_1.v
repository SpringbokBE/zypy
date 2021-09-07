// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================

`timescale 1 ns / 1 ps

module ufunc_call_f4_mul_23s_22ns_45_1_1_Multiplier_4(a, b, p);
input[23 - 1 : 0] a; 
input[22 - 1 : 0] b; 
output[45 - 1 : 0] p;

assign p = $signed(a) * $signed({1'b0, b});
endmodule
`timescale 1 ns / 1 ps
module ufunc_call_f4_mul_23s_22ns_45_1_1(
    din0,
    din1,
    dout);

parameter ID = 32'd1;
parameter NUM_STAGE = 32'd1;
parameter din0_WIDTH = 32'd1;
parameter din1_WIDTH = 32'd1;
parameter dout_WIDTH = 32'd1;
input[din0_WIDTH - 1:0] din0;
input[din1_WIDTH - 1:0] din1;
output[dout_WIDTH - 1:0] dout;



ufunc_call_f4_mul_23s_22ns_45_1_1_Multiplier_4 ufunc_call_f4_mul_23s_22ns_45_1_1_Multiplier_4_U(
    .a( din0 ),
    .b( din1 ),
    .p( dout ));

endmodule

