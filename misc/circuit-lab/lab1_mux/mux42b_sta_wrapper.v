module mux42b_sta_wrapper (
    input  wire clk,
    input  wire [1: 0] x0,
    input  wire [1: 0] x1,
    input  wire [1: 0] x2,
    input  wire [1: 0] x3,
    input  wire [1: 0] y,
    output wire [1: 0] f
  );

  reg [1: 0] x0_reg;
  reg [1: 0] x1_reg;
  reg [1: 0] x2_reg;
  reg [1: 0] x3_reg;
  reg [1: 0] y_reg;
  reg [1: 0] f_reg;
  wire f_inner;

  always @(posedge clk) begin
    x0_reg <= x0;
    x1_reg <= x1;
    x2_reg <= x2;
    x3_reg <= x3;
    y_reg <= y;
    f_reg <= f_inner;
  end

  assign f = f_reg;

  mux42b u0 (
    .x0 (x0_reg),
    .x1 (x1_reg),
    .x2 (x2_reg),
    .x3 (x3_reg),
    .y (y_reg),
    .f (f_inner)
  );

endmodule
