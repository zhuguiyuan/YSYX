module top (
    input  wire       clk,
    input  wire       rst,
    output wire [7:0] out0,
    output wire [7:0] out1
);

  wire [3:0] in0, in1;

  lfsr u_lfsr (
      .clk_i  (clk),
      .rst_i  (rst),
      .value_o({in1, in0})
  );

  bcd7seg u_bcd7seg_0 (
      .data_i(in0),
      .data_o(out0)
  );

  bcd7seg u_bcd7seg_1 (
      .data_i(in1),
      .data_o(out1)
  );

endmodule
