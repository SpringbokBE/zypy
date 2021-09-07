// ==============================================================
// RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Version: 2020.2
// Copyright (C) Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module ufunc_reduce_all_i4_exp_32_32_s (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
        ap_idle,
        ap_ready,
        ap_ce,
        x,
        ap_return
);

parameter    ap_ST_fsm_pp0_stage0 = 1'd1;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
input   ap_ce;
input  [31:0] x;
output  [30:0] ap_return;

reg ap_done;
reg ap_idle;
reg ap_ready;

(* fsm_encoding = "none" *) reg   [0:0] ap_CS_fsm;
wire    ap_CS_fsm_pp0_stage0;
wire    ap_enable_reg_pp0_iter0;
wire    ap_block_pp0_stage0;
reg    ap_enable_reg_pp0_iter1;
reg    ap_idle_pp0;
wire    ap_block_state1_pp0_stage0_iter0;
wire    ap_block_state2_pp0_stage0_iter1;
wire    ap_block_pp0_stage0_11001;
wire   [7:0] exp_x_msb_1_table_V_address0;
reg    exp_x_msb_1_table_V_ce0;
wire   [67:0] exp_x_msb_1_table_V_q0;
wire   [0:0] p_Result_26_fu_327_p3;
reg   [0:0] p_Result_26_reg_729;
wire   [0:0] or_ln571_6_fu_531_p2;
reg   [0:0] or_ln571_6_reg_734;
wire   [0:0] or_ln571_12_fu_567_p2;
reg   [0:0] or_ln571_12_reg_739;
wire   [0:0] or_ln571_19_fu_603_p2;
reg   [0:0] or_ln571_19_reg_744;
wire   [0:0] or_ln571_24_fu_633_p2;
reg   [0:0] or_ln571_24_reg_749;
reg    ap_block_pp0_stage0_subdone;
wire   [63:0] zext_ln488_fu_653_p1;
wire   [0:0] p_Result_s_fu_119_p3;
wire   [0:0] p_Result_1_fu_127_p3;
wire   [0:0] p_Result_2_fu_135_p3;
wire   [0:0] p_Result_3_fu_143_p3;
wire   [0:0] p_Result_4_fu_151_p3;
wire   [0:0] p_Result_5_fu_159_p3;
wire   [0:0] p_Result_6_fu_167_p3;
wire   [0:0] p_Result_7_fu_175_p3;
wire   [0:0] p_Result_8_fu_183_p3;
wire   [0:0] p_Result_9_fu_191_p3;
wire   [0:0] p_Result_10_fu_199_p3;
wire   [0:0] p_Result_11_fu_207_p3;
wire   [0:0] p_Result_12_fu_215_p3;
wire   [0:0] p_Result_13_fu_223_p3;
wire   [0:0] p_Result_14_fu_231_p3;
wire   [0:0] p_Result_15_fu_239_p3;
wire   [0:0] p_Result_16_fu_247_p3;
wire   [0:0] p_Result_17_fu_255_p3;
wire   [0:0] p_Result_18_fu_263_p3;
wire   [0:0] p_Result_19_fu_271_p3;
wire   [0:0] p_Result_20_fu_279_p3;
wire   [0:0] p_Result_21_fu_287_p3;
wire   [0:0] p_Result_22_fu_295_p3;
wire   [0:0] p_Result_23_fu_303_p3;
wire   [0:0] p_Result_24_fu_311_p3;
wire   [0:0] p_Result_25_fu_319_p3;
wire   [5:0] p_Result_27_fu_335_p1;
wire   [0:0] overf_fu_339_p2;
wire   [0:0] xor_ln176_1_fu_351_p2;
wire   [0:0] or_ln571_1_fu_501_p2;
wire   [0:0] xor_ln176_fu_345_p2;
wire   [0:0] xor_ln176_2_fu_357_p2;
wire   [0:0] xor_ln176_3_fu_363_p2;
wire   [0:0] xor_ln176_4_fu_369_p2;
wire   [0:0] xor_ln176_5_fu_375_p2;
wire   [0:0] or_ln571_4_fu_519_p2;
wire   [0:0] or_ln571_3_fu_513_p2;
wire   [0:0] or_ln571_5_fu_525_p2;
wire   [0:0] or_ln571_2_fu_507_p2;
wire   [0:0] xor_ln176_7_fu_387_p2;
wire   [0:0] xor_ln176_8_fu_393_p2;
wire   [0:0] or_ln571_7_fu_537_p2;
wire   [0:0] xor_ln176_6_fu_381_p2;
wire   [0:0] xor_ln176_9_fu_399_p2;
wire   [0:0] xor_ln176_10_fu_405_p2;
wire   [0:0] xor_ln176_11_fu_411_p2;
wire   [0:0] xor_ln176_12_fu_417_p2;
wire   [0:0] or_ln571_10_fu_555_p2;
wire   [0:0] or_ln571_9_fu_549_p2;
wire   [0:0] or_ln571_11_fu_561_p2;
wire   [0:0] or_ln571_8_fu_543_p2;
wire   [0:0] xor_ln176_14_fu_429_p2;
wire   [0:0] xor_ln176_15_fu_435_p2;
wire   [0:0] or_ln571_14_fu_573_p2;
wire   [0:0] xor_ln176_13_fu_423_p2;
wire   [0:0] xor_ln176_16_fu_441_p2;
wire   [0:0] xor_ln176_17_fu_447_p2;
wire   [0:0] xor_ln176_18_fu_453_p2;
wire   [0:0] xor_ln176_19_fu_459_p2;
wire   [0:0] or_ln571_17_fu_591_p2;
wire   [0:0] or_ln571_16_fu_585_p2;
wire   [0:0] or_ln571_18_fu_597_p2;
wire   [0:0] or_ln571_15_fu_579_p2;
wire   [0:0] xor_ln176_21_fu_471_p2;
wire   [0:0] xor_ln176_22_fu_477_p2;
wire   [0:0] or_ln571_20_fu_609_p2;
wire   [0:0] xor_ln176_20_fu_465_p2;
wire   [0:0] xor_ln176_23_fu_483_p2;
wire   [0:0] xor_ln176_24_fu_489_p2;
wire   [0:0] icmp_ln1448_fu_495_p2;
wire   [0:0] or_ln571_22_fu_621_p2;
wire   [0:0] or_ln571_23_fu_627_p2;
wire   [0:0] or_ln571_21_fu_615_p2;
wire   [4:0] trunc_ln368_fu_639_p1;
wire   [7:0] p_Result_28_fu_643_p4;
wire   [0:0] or_ln571_25_fu_662_p2;
wire   [0:0] or_ln571_13_fu_658_p2;
wire   [0:0] xor_ln803_fu_672_p2;
wire   [0:0] or_ln571_fu_666_p2;
wire   [63:0] select_ln368_fu_677_p3;
wire   [63:0] y_V_fu_685_p4;
wire   [63:0] p_Val2_s_fu_695_p3;
wire   [0:0] p_Result_29_fu_703_p3;
wire   [30:0] tmp_fu_711_p4;
reg   [0:0] ap_NS_fsm;
reg    ap_idle_pp0_0to0;
reg    ap_reset_idle_pp0;
wire    ap_enable_pp0;
wire    ap_ce_reg;

// power-on initialization
initial begin
#0 ap_CS_fsm = 1'd1;
#0 ap_enable_reg_pp0_iter1 = 1'b0;
end

ufunc_reduce_all_i4_exp_32_32_s_exp_x_msb_1_table_V #(
    .DataWidth( 68 ),
    .AddressRange( 256 ),
    .AddressWidth( 8 ))
exp_x_msb_1_table_V_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .address0(exp_x_msb_1_table_V_address0),
    .ce0(exp_x_msb_1_table_V_ce0),
    .q0(exp_x_msb_1_table_V_q0)
);

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_fsm_pp0_stage0;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter1 <= 1'b0;
    end else begin
        if (((1'b0 == ap_block_pp0_stage0_subdone) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
            ap_enable_reg_pp0_iter1 <= ap_start;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_ce) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        or_ln571_12_reg_739 <= or_ln571_12_fu_567_p2;
        or_ln571_19_reg_744 <= or_ln571_19_fu_603_p2;
        or_ln571_24_reg_749 <= or_ln571_24_fu_633_p2;
        or_ln571_6_reg_734 <= or_ln571_6_fu_531_p2;
        p_Result_26_reg_729 <= x[32'd31];
    end
end

always @ (*) begin
    if ((((ap_start == 1'b0) & (1'b0 == ap_block_pp0_stage0) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter0 == 1'b1)) | ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_ce) & (1'b1 == ap_CS_fsm_pp0_stage0)))) begin
        ap_done = 1'b1;
    end else begin
        ap_done = 1'b0;
    end
end

always @ (*) begin
    if (((ap_start == 1'b0) & (ap_idle_pp0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter1 == 1'b0) & (ap_enable_reg_pp0_iter0 == 1'b0))) begin
        ap_idle_pp0 = 1'b1;
    end else begin
        ap_idle_pp0 = 1'b0;
    end
end

always @ (*) begin
    if ((ap_enable_reg_pp0_iter0 == 1'b0)) begin
        ap_idle_pp0_0to0 = 1'b1;
    end else begin
        ap_idle_pp0_0to0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_ce) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter0 == 1'b1))) begin
        ap_ready = 1'b1;
    end else begin
        ap_ready = 1'b0;
    end
end

always @ (*) begin
    if (((ap_start == 1'b0) & (ap_idle_pp0_0to0 == 1'b1))) begin
        ap_reset_idle_pp0 = 1'b1;
    end else begin
        ap_reset_idle_pp0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_ce) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter0 == 1'b1))) begin
        exp_x_msb_1_table_V_ce0 = 1'b1;
    end else begin
        exp_x_msb_1_table_V_ce0 = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_pp0_stage0 : begin
            ap_NS_fsm = ap_ST_fsm_pp0_stage0;
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign ap_CS_fsm_pp0_stage0 = ap_CS_fsm[32'd0];

assign ap_block_pp0_stage0 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_11001 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_pp0_stage0_subdone = (1'b0 == ap_ce);
end

assign ap_block_state1_pp0_stage0_iter0 = ~(1'b1 == 1'b1);

assign ap_block_state2_pp0_stage0_iter1 = ~(1'b1 == 1'b1);

assign ap_enable_pp0 = (ap_idle_pp0 ^ 1'b1);

assign ap_enable_reg_pp0_iter0 = ap_start;

assign ap_return = ((p_Result_29_fu_703_p3[0:0] == 1'b1) ? 31'd2147483647 : tmp_fu_711_p4);

assign exp_x_msb_1_table_V_address0 = zext_ln488_fu_653_p1;

assign icmp_ln1448_fu_495_p2 = (($signed(p_Result_27_fu_335_p1) > $signed(6'd22)) ? 1'b1 : 1'b0);

assign or_ln571_10_fu_555_p2 = (xor_ln176_12_fu_417_p2 | xor_ln176_11_fu_411_p2);

assign or_ln571_11_fu_561_p2 = (or_ln571_9_fu_549_p2 | or_ln571_10_fu_555_p2);

assign or_ln571_12_fu_567_p2 = (or_ln571_8_fu_543_p2 | or_ln571_11_fu_561_p2);

assign or_ln571_13_fu_658_p2 = (or_ln571_6_reg_734 | or_ln571_12_reg_739);

assign or_ln571_14_fu_573_p2 = (xor_ln176_15_fu_435_p2 | xor_ln176_14_fu_429_p2);

assign or_ln571_15_fu_579_p2 = (xor_ln176_13_fu_423_p2 | or_ln571_14_fu_573_p2);

assign or_ln571_16_fu_585_p2 = (xor_ln176_17_fu_447_p2 | xor_ln176_16_fu_441_p2);

assign or_ln571_17_fu_591_p2 = (xor_ln176_19_fu_459_p2 | xor_ln176_18_fu_453_p2);

assign or_ln571_18_fu_597_p2 = (or_ln571_17_fu_591_p2 | or_ln571_16_fu_585_p2);

assign or_ln571_19_fu_603_p2 = (or_ln571_18_fu_597_p2 | or_ln571_15_fu_579_p2);

assign or_ln571_1_fu_501_p2 = (xor_ln176_1_fu_351_p2 | overf_fu_339_p2);

assign or_ln571_20_fu_609_p2 = (xor_ln176_22_fu_477_p2 | xor_ln176_21_fu_471_p2);

assign or_ln571_21_fu_615_p2 = (xor_ln176_20_fu_465_p2 | or_ln571_20_fu_609_p2);

assign or_ln571_22_fu_621_p2 = (xor_ln176_24_fu_489_p2 | xor_ln176_23_fu_483_p2);

assign or_ln571_23_fu_627_p2 = (or_ln571_22_fu_621_p2 | icmp_ln1448_fu_495_p2);

assign or_ln571_24_fu_633_p2 = (or_ln571_23_fu_627_p2 | or_ln571_21_fu_615_p2);

assign or_ln571_25_fu_662_p2 = (or_ln571_24_reg_749 | or_ln571_19_reg_744);

assign or_ln571_2_fu_507_p2 = (xor_ln176_fu_345_p2 | or_ln571_1_fu_501_p2);

assign or_ln571_3_fu_513_p2 = (xor_ln176_3_fu_363_p2 | xor_ln176_2_fu_357_p2);

assign or_ln571_4_fu_519_p2 = (xor_ln176_5_fu_375_p2 | xor_ln176_4_fu_369_p2);

assign or_ln571_5_fu_525_p2 = (or_ln571_4_fu_519_p2 | or_ln571_3_fu_513_p2);

assign or_ln571_6_fu_531_p2 = (or_ln571_5_fu_525_p2 | or_ln571_2_fu_507_p2);

assign or_ln571_7_fu_537_p2 = (xor_ln176_8_fu_393_p2 | xor_ln176_7_fu_387_p2);

assign or_ln571_8_fu_543_p2 = (xor_ln176_6_fu_381_p2 | or_ln571_7_fu_537_p2);

assign or_ln571_9_fu_549_p2 = (xor_ln176_9_fu_399_p2 | xor_ln176_10_fu_405_p2);

assign or_ln571_fu_666_p2 = (or_ln571_25_fu_662_p2 | or_ln571_13_fu_658_p2);

assign overf_fu_339_p2 = (p_Result_s_fu_119_p3 ^ p_Result_26_fu_327_p3);

assign p_Result_10_fu_199_p3 = x[32'd15];

assign p_Result_11_fu_207_p3 = x[32'd16];

assign p_Result_12_fu_215_p3 = x[32'd17];

assign p_Result_13_fu_223_p3 = x[32'd18];

assign p_Result_14_fu_231_p3 = x[32'd19];

assign p_Result_15_fu_239_p3 = x[32'd20];

assign p_Result_16_fu_247_p3 = x[32'd21];

assign p_Result_17_fu_255_p3 = x[32'd22];

assign p_Result_18_fu_263_p3 = x[32'd23];

assign p_Result_19_fu_271_p3 = x[32'd24];

assign p_Result_1_fu_127_p3 = x[32'd6];

assign p_Result_20_fu_279_p3 = x[32'd25];

assign p_Result_21_fu_287_p3 = x[32'd26];

assign p_Result_22_fu_295_p3 = x[32'd27];

assign p_Result_23_fu_303_p3 = x[32'd28];

assign p_Result_24_fu_311_p3 = x[32'd29];

assign p_Result_25_fu_319_p3 = x[32'd30];

assign p_Result_26_fu_327_p3 = x[32'd31];

assign p_Result_27_fu_335_p1 = x[5:0];

assign p_Result_28_fu_643_p4 = {{{p_Result_26_fu_327_p3}, {trunc_ln368_fu_639_p1}}, {2'd0}};

assign p_Result_29_fu_703_p3 = p_Val2_s_fu_695_p3[32'd63];

assign p_Result_2_fu_135_p3 = x[32'd7];

assign p_Result_3_fu_143_p3 = x[32'd8];

assign p_Result_4_fu_151_p3 = x[32'd9];

assign p_Result_5_fu_159_p3 = x[32'd10];

assign p_Result_6_fu_167_p3 = x[32'd11];

assign p_Result_7_fu_175_p3 = x[32'd12];

assign p_Result_8_fu_183_p3 = x[32'd13];

assign p_Result_9_fu_191_p3 = x[32'd14];

assign p_Result_s_fu_119_p3 = x[32'd5];

assign p_Val2_s_fu_695_p3 = ((or_ln571_fu_666_p2[0:0] == 1'b1) ? select_ln368_fu_677_p3 : y_V_fu_685_p4);

assign select_ln368_fu_677_p3 = ((xor_ln803_fu_672_p2[0:0] == 1'b1) ? 64'd18446744073709551615 : 64'd0);

assign tmp_fu_711_p4 = {{p_Val2_s_fu_695_p3[62:32]}};

assign trunc_ln368_fu_639_p1 = x[4:0];

assign xor_ln176_10_fu_405_p2 = (p_Result_26_fu_327_p3 ^ p_Result_11_fu_207_p3);

assign xor_ln176_11_fu_411_p2 = (p_Result_26_fu_327_p3 ^ p_Result_12_fu_215_p3);

assign xor_ln176_12_fu_417_p2 = (p_Result_26_fu_327_p3 ^ p_Result_13_fu_223_p3);

assign xor_ln176_13_fu_423_p2 = (p_Result_26_fu_327_p3 ^ p_Result_14_fu_231_p3);

assign xor_ln176_14_fu_429_p2 = (p_Result_26_fu_327_p3 ^ p_Result_15_fu_239_p3);

assign xor_ln176_15_fu_435_p2 = (p_Result_26_fu_327_p3 ^ p_Result_16_fu_247_p3);

assign xor_ln176_16_fu_441_p2 = (p_Result_26_fu_327_p3 ^ p_Result_17_fu_255_p3);

assign xor_ln176_17_fu_447_p2 = (p_Result_26_fu_327_p3 ^ p_Result_18_fu_263_p3);

assign xor_ln176_18_fu_453_p2 = (p_Result_26_fu_327_p3 ^ p_Result_19_fu_271_p3);

assign xor_ln176_19_fu_459_p2 = (p_Result_26_fu_327_p3 ^ p_Result_20_fu_279_p3);

assign xor_ln176_1_fu_351_p2 = (p_Result_2_fu_135_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_20_fu_465_p2 = (p_Result_26_fu_327_p3 ^ p_Result_21_fu_287_p3);

assign xor_ln176_21_fu_471_p2 = (p_Result_26_fu_327_p3 ^ p_Result_22_fu_295_p3);

assign xor_ln176_22_fu_477_p2 = (p_Result_26_fu_327_p3 ^ p_Result_23_fu_303_p3);

assign xor_ln176_23_fu_483_p2 = (p_Result_26_fu_327_p3 ^ p_Result_24_fu_311_p3);

assign xor_ln176_24_fu_489_p2 = (p_Result_26_fu_327_p3 ^ p_Result_25_fu_319_p3);

assign xor_ln176_2_fu_357_p2 = (p_Result_3_fu_143_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_3_fu_363_p2 = (p_Result_4_fu_151_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_4_fu_369_p2 = (p_Result_5_fu_159_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_5_fu_375_p2 = (p_Result_6_fu_167_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_6_fu_381_p2 = (p_Result_7_fu_175_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_7_fu_387_p2 = (p_Result_8_fu_183_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_8_fu_393_p2 = (p_Result_9_fu_191_p3 ^ p_Result_26_fu_327_p3);

assign xor_ln176_9_fu_399_p2 = (p_Result_26_fu_327_p3 ^ p_Result_10_fu_199_p3);

assign xor_ln176_fu_345_p2 = (p_Result_26_fu_327_p3 ^ p_Result_1_fu_127_p3);

assign xor_ln803_fu_672_p2 = (p_Result_26_reg_729 ^ 1'd1);

assign y_V_fu_685_p4 = {{exp_x_msb_1_table_V_q0[67:4]}};

assign zext_ln488_fu_653_p1 = p_Result_28_fu_643_p4;

endmodule //ufunc_reduce_all_i4_exp_32_32_s
