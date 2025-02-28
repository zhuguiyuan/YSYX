module top(
    input  wire        en,
    input  wire [7: 0] in,
    output wire [3: 0] encoded,
    output wire [7: 0] seg
  );

  priority_encoder #(
    .INPUT_WIDTH (8)
  ) u_priority_encoder (
    .en (en),
    .in (in),
    .out (encoded[2:0]),
    .in_zero (encoded[3])
  );

  bcd7seg u_bcd7seg (
    .in ({1'b0, encoded[2:0]}),
    .out (seg)
  );


endmodule
