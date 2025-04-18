module bcd7seg (
    input  wire        en,
    input  wire [3: 0] in,
    output wire [7: 0] out
  );

  wire [7: 0] segs [0: 15];
  assign segs[4'h0] = 8'b11111100;
  assign segs[4'h1] = 8'b01100000;
  assign segs[4'h2] = 8'b11011010;
  assign segs[4'h3] = 8'b11110010;
  assign segs[4'h4] = 8'b01100110;
  assign segs[4'h5] = 8'b10110110;
  assign segs[4'h6] = 8'b10111110;
  assign segs[4'h7] = 8'b11100000;
  assign segs[4'h8] = 8'b11111110;
  assign segs[4'h9] = 8'b11110110;
  assign segs[4'hA] = 8'b11101110;
  assign segs[4'hB] = 8'b00111110;
  assign segs[4'hC] = 8'b10011100;
  assign segs[4'hD] = 8'b01111010;
  assign segs[4'hE] = 8'b10011110;
  assign segs[4'hF] = 8'b10001110;
  assign out = en ? ~segs[in] : ~8'b00000000;

endmodule
