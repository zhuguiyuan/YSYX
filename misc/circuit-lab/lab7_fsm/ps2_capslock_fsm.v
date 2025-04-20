module ps2_capslock_fsm (
    input  wire clk,
    input  wire rstn,
    input  wire key_capslock_p,
    output wire capslock_s
);
  reg status_reg = 0;
  always @(posedge clk) begin
    if (~rstn) begin
      status_reg <= 0;
    end else if (key_capslock_p) begin
      status_reg <= ~status_reg;
    end
  end
  assign capslock_s = status_reg;
endmodule
