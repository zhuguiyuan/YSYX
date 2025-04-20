module sta_wrapper(
    input  wire        clk,
    input  wire        en,
    input  wire [7: 0] in,
    output wire [3: 0] encoded,
    output wire [7: 0] seg
  );

  reg         en_reg;
  reg  [7: 0] in_reg;
  reg  [3: 0] encoded_reg;
  reg  [7: 0] seg_reg;
  wire [3: 0] encoded_inner;
  wire [7: 0] seg_inner;

  top u_top (
    .en (en_reg),
    .in (in_reg),
    .encoded (encoded_inner),
    .seg (seg_inner)
  );

  always @(posedge clk) begin
    en_reg <= en;
    in_reg <= in;
    encoder_reg <= encoded_inner;
    seg_reg <= seg_inner;
  end

  assign encoded = encoded_reg;
  assign seg = seg_reg;

endmodule

