module top (
    input  wire       en_i,
    input  wire [7:0] data_i,
    output wire [3:0] data_o,
    output wire [7:0] seg_o
);

  priority_encoder #(
      .INPUT_WIDTH(8)
  ) u_priority_encoder (
      .en_i(en_i),
      .data_i(data_i),
      .data_o(data_o[2:0]),
      .zero_flag_no(data_o[3])
  );

  bcd7seg u_bcd7seg (
      .data_i({1'b0, data_o[2:0]}),
      .data_o(seg_o)
  );


endmodule
