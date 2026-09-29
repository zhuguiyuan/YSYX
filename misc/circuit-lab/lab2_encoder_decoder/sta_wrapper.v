module sta_wrapper (
    input  wire       clk,
    input  wire       en_i,
    input  wire [7:0] data_i,
    output wire [3:0] data_o,
    output wire [7:0] seg_o
);

  reg        en_reg;
  reg  [7:0] in_reg;
  reg  [3:0] encoded_reg;
  reg  [7:0] seg_reg;
  wire [3:0] encoded_inner;
  wire [7:0] seg_inner;

  top u_top (
      .en_i  (en_reg),
      .data_i(in_reg),
      .data_o(encoded_inner),
      .seg_o (seg_inner)
  );

  always @(posedge clk) begin
    en_reg <= en_i;
    in_reg <= data_i;
    encoded_reg <= encoded_inner;
    seg_reg <= seg_inner;
  end

  assign data_o = encoded_reg;
  assign seg_o  = seg_reg;

endmodule

