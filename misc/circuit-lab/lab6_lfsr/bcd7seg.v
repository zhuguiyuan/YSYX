module bcd7seg (
    input  wire [3:0] data_i,
    output wire [7:0] data_o
);

  MuxKeyWithDefault #(
      .NR_KEY  (10),
      .KEY_LEN (4),
      .DATA_LEN(8)
  ) rom (
      .key_i(data_i),
      .value_i(data_o),
      .default_i(~8'b1),
      .lut_i({
        4'd0, ~8'b11111100,
        4'd1, ~8'b01100000,
        4'd2, ~8'b11011010,
        4'd3, ~8'b11110010,
        4'd4, ~8'b01100110,
        4'd5, ~8'b10110110,
        4'd6, ~8'b10111110,
        4'd7, ~8'b11100000,
        4'd8, ~8'b11111110,
        4'd9, ~8'b11110110
      })
  );

endmodule
