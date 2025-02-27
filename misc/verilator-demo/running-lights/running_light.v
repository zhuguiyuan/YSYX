module running_light(
    input wire clk,
    input wire rst,
    output reg [15: 0] led
  );

  reg [31: 0]	count_reg;

  always @(posedge clk) begin
    if (rst) begin
      led <= 1;
      count_reg <= 0;
    end
    else begin
      if (count_reg == 0 )
        led <= {led[14: 0], led[15]};
      count_reg <= (count_reg >= 5000000 ? 32'b0 : count_reg + 1);
    end
  end

endmodule
