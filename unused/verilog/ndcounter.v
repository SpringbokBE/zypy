`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/18/2021 06:34:33 PM
// Design Name: 
// Module Name: counter
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module counter_modN #(
    parameter N = 10,  
    parameter WIDTH = 32
 ) (
    input clk,
    input inc,
    input rst,
    output reg max = 0,
    output reg[WIDTH - 1:0] count = 0
);

always @(posedge clk or posedge rst)
begin
    if(rst)
        count <= 0;
    else
    begin
        if(count < N - 1)
        begin
            count <= count + 1;
            max <= 0;
        end
        else
        begin
            count <= 0;
            max <= 1;
        end
    end
end
endmodule

module ripple # (
    parameter WIDTH = 32
) (
    input clk,
    input reset,
    output [WIDTH - 1:0] ndindex_0,
    output [WIDTH - 1:0] ndindex_1,
    output [WIDTH - 1:0] ndindex_2
);

wire max0;
wire max1;

counter_modN counter_0 (
    .clk(clk),
    .rst(reset),
    .max(max0),
    .count(ndindex_0)
);

counter_modN counter_1 (
    .clk(max0),
    .rst(reset),
    .max(max1),
    .count(ndindex_1)
);

counter_modN counter_2 (
    .clk(max1),
    .rst(reset),
    .max(max2),
    .count(ndindex_2)
);

endmodule