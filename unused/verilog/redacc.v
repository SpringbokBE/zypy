`timescale 1ns / 1ps

module counter_modN #(
    parameter WIDTH = 32
 ) (
    input clk,
    input inc,
    input rst,
    input [WIDTH - 1:0] N,
    output reg ovf = 0,
    output reg max = 0,
    output reg[WIDTH - 1:0] count = 0
);

always @(posedge clk or posedge rst)
begin
    if(rst)
        count <= 0;
    else
    begin
        if(count == N - 2 || N == 1)
            max <= 1;
        else
            max <= 0;    
        if (count == N - 1)
        begin
            count <= 0;
            ovf <= 1;
        end
        else
        begin
            count <= count + 1;
            ovf <= 0;
        end
    end
end
endmodule

// By experiment, we need a two cycle delay after a reset.

module counter_modN_delayed #(
    parameter WIDTH = 32
 ) (
    input clk,
    input inc,
    input rst,
    input [WIDTH - 1:0] N,
    output reg ovf = 0,
    output reg max = 0,
    output reg[WIDTH - 1:0] count = 0
);

reg delay2 = 1;
reg delay1 = 0;

always @(posedge clk or posedge rst)
begin
    if(rst)
    begin
        count <= 0;
        delay1 <= 0;
        delay2 <= 1;
    end
    else if(delay2)
    begin
        delay1 <= 1;
        delay2 <= 0;
    end
    else if(delay1)
        delay1 <= 0;
    else
    begin
        if(count == N - 2)
            max <= 1;
        else
            max <= 0;    
        if (count == N - 1)
        begin
            count <= 0;
            ovf <= 1;
        end
        else
        begin
            count <= count + 1;
            ovf <= 0;
        end
    end
end
endmodule

module redacc # (
    parameter INSTANCES = 4,
    parameter WIDTH = 32
) (
    input clk,
    input rst,
    input [INSTANCES * WIDTH - 1:0] shape,
    input [INSTANCES - 1:0] axes,
    input [INSTANCES * WIDTH - 1:0] deltas,
    output [INSTANCES * WIDTH - 1:0] ndindex,
    output [INSTANCES - 1:0] zero,
    output [INSTANCES - 1:0] max,
    output bypass,
    output stream_out,
    output [WIDTH - 1:0] n_out
);

wire [INSTANCES - 1:0] ovf;
//wire [INSTANCES - 1:0] idx;

counter_modN_delayed counter_0 (
    .clk(clk),
    .rst(rst),
    .N(shape[WIDTH - 1:0]),
    .ovf(ovf[0]),
    .max(max[0]),
    .count(ndindex[WIDTH - 1:0])
);

assign zero[0] = (ndindex[WIDTH - 1:0] == 0);

generate genvar i;
    for (i = 1; i < INSTANCES; i = i + 1)
    begin
        counter_modN inst (
            .clk(ovf[i - 1]),
            .rst(rst),
            .N(shape[(i + 1) * WIDTH - 1:i * WIDTH]),
            .ovf(ovf[i]),
            .max(max[i]),
            .count(ndindex[(i + 1) * WIDTH - 1:i * WIDTH])
        );
        assign zero[i] = (ndindex[(i + 1) * WIDTH - 1:i * WIDTH] == 0);
    end
endgenerate

assign bypass = ((axes & zero) == axes);
assign stream_out = ((axes & max) == axes);

// See https://stackoverflow.com/a/38258865
// This is the reversed implementation

wire [INSTANCES - 1:0] out_stage [0:INSTANCES];
assign n_out = 0;
assign out_stage[INSTANCES] = 0; // desired default output if no bits set

generate genvar j;
    for(j = INSTANCES - 1; j != ~INSTANCES; j = j - 1)
//        assign out_stage[j] = ovf[j] ? out_stage[j + 1] : j;
        assign out_stage[j] = ovf[j] ? out_stage[j + 1] : n_out + deltas[(j + 1) * WIDTH - 1:j * WIDTH];
endgenerate

//assign idx = out_stage[0];
assign n_out = out_stage[0];

endmodule