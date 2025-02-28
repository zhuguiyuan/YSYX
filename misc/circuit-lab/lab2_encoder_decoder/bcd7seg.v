module bcd7seg(
    input  wire [3: 0] in,
    output wire [7: 0] out
  );

  wire [7: 0] segs [0: 9];
  assign segs[0] = 8'b11111100;
  assign segs[1] = 8'b01100000;
  assign segs[2] = 8'b11011010;
  assign segs[3] = 8'b11110010;
  assign segs[4] = 8'b01100110;
  assign segs[5] = 8'b10110110;
  assign segs[6] = 8'b10111110;
  assign segs[7] = 8'b11100000;
  assign segs[8] = 8'b11111110;
  assign segs[9] = 8'b11110110;
  assign out = in < 10 ? ~segs[in] : ~8'b000000100;

endmodule
