module priority_encoder #(
    parameter INPUT_WIDTH  = 8,
    parameter OUTPUT_WIDTH = $clog2(INPUT_WIDTH)
) (
    input  wire                      en_i,
    input  wire [ INPUT_WIDTH - 1:0] data_i,
    output wire [OUTPUT_WIDTH - 1:0] data_o,
    output wire                      zero_flag_no
);

  reg [OUTPUT_WIDTH - 1:0] encode_result;
  integer i;
  always @(*) begin
    encode_result = 0;
    for (i = 0; i < INPUT_WIDTH; i = i + 1) begin
      if (data_i[i] == 1) encode_result = i[OUTPUT_WIDTH-1:0];
    end
  end
  /* verilator lint_off CASEX */
  // always @(*) begin
  //   casex(data_i)
  //     8'b00000000: encode_result = 3'd0;
  //     8'b00000001: encode_result = 3'd0;
  //     8'b0000001x: encode_result = 3'd1;
  //     8'b000001xx: encode_result = 3'd2;
  //     8'b00001xxx: encode_result = 3'd3;
  //     8'b0001xxxx: encode_result = 3'd4;
  //     8'b001xxxxx: encode_result = 3'd5;
  //     8'b01xxxxxx: encode_result = 3'd6;
  //     8'b1xxxxxxx: encode_result = 3'd7;
  //   endcase
  // end
  /* verilator lint_on CASEX */
  assign data_o = en_i ? encode_result : 0;
  assign zero_flag_no = en_i && data_i != 0;

endmodule
