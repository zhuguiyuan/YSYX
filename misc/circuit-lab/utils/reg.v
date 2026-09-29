// 触发器模板
module Reg #(
    WIDTH     = 1,
    RESET_VAL = 0
) (
    input                  clk_i,
    input                  rst_i,
    input      [WIDTH-1:0] din_i,
    output reg [WIDTH-1:0] dout_o,
    input                  wen_i
);
  always @(posedge clk_i) begin
    if (rst_i) dout_o <= RESET_VAL;
    else if (wen_i) dout_o <= din_i;
  end
endmodule
