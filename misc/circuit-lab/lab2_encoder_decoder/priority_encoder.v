module priority_encoder #(
    parameter INPUT_WIDTH = 8,
    parameter OUTPUT_WIDTH = $clog2(INPUT_WIDTH)
  ) (
    input  wire                       en,
    input  wire [ INPUT_WIDTH - 1: 0] in,
    output wire [OUTPUT_WIDTH - 1: 0] out,
    output wire                       in_zero
  );

  reg [OUTPUT_WIDTH - 1: 0] out_inner;
  integer i;
  always @( * ) begin
    out_inner = 0;
    for (i = 0; i < INPUT_WIDTH; i = i + 1) begin
      if (in[i] == 1)
        out_inner = i[OUTPUT_WIDTH - 1: 0];
    end
  end
  assign out = en ? out_inner : 0;
  assign in_zero = ~en || in == 0;

endmodule
