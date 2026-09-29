module mux42b (
    input  wire [1:0] x0,
    input  wire [1:0] x1,
    input  wire [1:0] x2,
    input  wire [1:0] x3,
    input  wire [1:0] y,
    output wire [1:0] f
);

  MuxKeyWithDefault #(
      .NR_KEY  (4),
      .KEY_LEN (2),
      .DATA_LEN(2)
  ) i0 (
      .out_o(f),
      .key_i(y),
      .default_out_i(0),
      .lut_i({2'b00, x0, 2'b01, x1, 2'b10, x2, 2'b11, x3})
  );

endmodule
