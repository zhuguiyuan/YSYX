module ps2_control_key_fsm (
    input  wire clk,
    input  wire rstn,
    input  wire key_p,
    input  wire key_r,
    output wire key_s
);
  reg pressing_reg = 0;
  always @(posedge clk) begin
    if (~rstn) begin
      pressing_reg <= 0;
    end else if (key_p) begin
      pressing_reg <= 1;
    end else if (key_r) begin
      pressing_reg <= 0;
    end
  end
  assign key_s = pressing_reg;
endmodule
