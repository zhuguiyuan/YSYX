module ps2_normal_key_fsm (
    input  wire       clk,
    input  wire       rstn,
    input  wire [7:0] key_d,
    input  wire       key_p,
    input  wire       key_r,
    output wire       key_s,
    output wire [7:0] code
);
  reg [7:0] code_reg = 0;
  /* verilator lint_off UNUSEDSIGNAL */
  reg [7:0] ascii_reg = 0;
  reg       pressing_reg = 0;
  always @(posedge clk) begin
    if (~rstn) begin
      code_reg <= 0;
      pressing_reg <= 0;
    end else if (~pressing_reg && key_p) begin
      code_reg <= key_d;
      pressing_reg <= 1;
    end else if (pressing_reg && key_r && key_d == code_reg) begin
      pressing_reg <= 0;
      code_reg <= 0;
    end
  end
  assign key_s = pressing_reg;
  assign code  = code_reg;
endmodule
