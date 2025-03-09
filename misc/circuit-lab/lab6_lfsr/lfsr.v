module lfsr (
    input  wire        clk,
    input  wire        rst,
    output wire [7: 0] value
  );

  reg [7: 0] lfsr_reg;
  wire new_bit = lfsr_reg[4] ^ lfsr_reg[3] ^ lfsr_reg[2] ^ lfsr_reg[0];

  always @(posedge clk) begin
    lfsr_reg <= {new_bit, lfsr_reg[7: 1]};
    if (rst) begin
      lfsr_reg <= 8'b00000001;
    end
  end

  assign value = lfsr_reg;

endmodule
