module lfsr (
    input  wire       clk_i,
    input  wire       rst_i,
    output wire [7:0] value_o
);

  reg [7:0] lfsr_reg;
  wire new_bit = lfsr_reg[4] ^ lfsr_reg[3] ^ lfsr_reg[2] ^ lfsr_reg[0];

  always @(posedge clk_i) begin
    if (rst_i) begin
      lfsr_reg <= 8'b00000001;
    end else begin
      lfsr_reg <= {new_bit, lfsr_reg[7:1]};
    end
  end

  assign value_o = lfsr_reg;

endmodule
