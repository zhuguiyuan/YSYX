module sta_wrapper (
    input  wire       clk,
    input  wire [2:0] op,
    input  wire [3:0] a,
    input  wire [3:0] b,
    output wire [3:0] r,
    output wire       cf,
    output wire       of,
    output wire       zf
);

  reg  [2:0] op_reg;
  reg  [3:0] a_reg;
  reg  [3:0] b_reg;
  reg  [3:0] r_reg;
  reg        c_reg;
  reg        of_reg;
  reg        z_reg;
  wire [3:0] r_inner;
  wire       cf_inner;
  wire       of_inner;
  wire       zf_inner;

  always @(posedge clk) begin
    op_reg <= op;
    a_reg <= a;
    b_reg <= b;
    r_reg <= r_inner;
    cf_reg <= cf_inner;
    of_reg <= of_inner;
    zf_reg <= zf_inner;
  end

  alu u_alu (
    .op (op_reg),
    .a (a_reg),
    .b (b_reg),
    .r (r_inner),
    .cf (cf_inner),
    .of (of_inner),
    .zf (zf_inner)
  );

  assign r = r_reg;
  assign cf = cf_reg;
  assign of = of_reg;
  assign zf = zf_reg;

endmodule
