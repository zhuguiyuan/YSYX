module top (
    input  wire clk,
    input  wire rst,
    input  wire enq_data,
    input  wire enq_valid,
    output wire enq_ready,
    output wire deq_data,
    output wire deq_valid,
    input  wire deq_ready
  );
  reg data_reg = 0;
  reg state_reg = 0;

  wire ctrl_enq = state_reg == 0 && enq_valid;
  wire ctrl_deq = state_reg == 1 && deq_ready;

  always @(posedge clk) begin
    if (ctrl_enq) begin
      data_reg <= enq_data;
      state_reg <= 1;
    end else if (ctrl_deq) begin
      data_reg <= 0;
      state_reg <= 0;
    end
    if (rst) begin
      data_reg <= 0;
      state_reg <= 0; 
    end
  end

  assign deq_data = data_reg;
  assign enq_ready = state_reg == 0;
  assign deq_valid = state_reg == 1;

endmodule 
