module top(
    input  wire        clk,
    input  wire        rst,
    output wire [7: 0] out0,
    output wire [7: 0] out1
  );

  wire [3:0] in0, in1;

  lfsr u_lfsr (
    .clk (clk),
    .rst (rst),
    .value ({in1, in0})
  );

  bcd7seg u_bcd7seg_0 (
    .in (in0),
    .out (out0)
  );

  bcd7seg u_bcd7seg_1 (
    .in (in1),
    .out (out1)
  );

endmodule
