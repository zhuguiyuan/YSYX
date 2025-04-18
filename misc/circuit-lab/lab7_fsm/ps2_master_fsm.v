module ps2_master_fsm (
    input  wire       clk,
    input  wire       rstn,
    input  wire [7:0] code_data,
    input  wire       code_valid,
    output reg        code_ready,
    output reg        key_lshift_p,
    output reg        key_lshift_r,
    output reg        key_rshift_p,
    output reg        key_rshift_r,
    output reg        key_alt_p,
    output reg        key_alt_r,
    output reg        key_ctrl_p,
    output reg        key_ctrl_r,
    output reg        key_capslock_p,
    output reg        key_capslock_r,
    output reg  [7:0] key_norm_d,
    output reg        key_norm_p,
    output reg        key_norm_r
);

  // verilog_format: off
  localparam PR_IDLE         = 4'b0001,
             PR_PRESS        = 4'b0010,
             PR_TO_RELEASE   = 4'b0100,
             PR_RELEASE      = 4'b1000;

  localparam _PR_IDLE_       = 0,
             _PR_PRESS_      = 1,
             _PR_TO_RELEASE_ = 2,
             _PR_RELEASE_    = 3;
  // verilog_format: on

  // controls
  wire ctrl_pr_fsm_fire = code_valid & code_ready;
  wire ctrl_pr_fsm_press = code_data != 8'hF0;
  wire ctrl_pr_fsm_release = ~ctrl_pr_fsm_press;

  // pr_fsm states
  reg [3:0] pr_fsm_reg = PR_IDLE, pr_fsm_next;
  // pr_fsm pressed or released key
  reg [7:0] pr_fsm_key_reg, pr_fsm_key_next;

  always @(posedge clk) begin : pr_fsm_state
    if (~rstn) begin
      pr_fsm_reg <= PR_IDLE;
      // no need to reset pr_fsm_key_reg
    end else begin
      pr_fsm_reg <= pr_fsm_next;
      pr_fsm_key_reg <= pr_fsm_key_next;
    end
  end

  always @(*) begin : pr_fsm_trans
    pr_fsm_next = pr_fsm_reg;
    pr_fsm_key_next = pr_fsm_key_reg;
    case (1'b1)
      pr_fsm_reg[_PR_IDLE_]: begin
        if (ctrl_pr_fsm_fire & ctrl_pr_fsm_press) begin
          pr_fsm_next = PR_PRESS;
          pr_fsm_key_next = code_data;
        end else if (ctrl_pr_fsm_fire & ctrl_pr_fsm_release) begin
          pr_fsm_next = PR_TO_RELEASE;
        end
      end
      pr_fsm_reg[_PR_PRESS_]: begin
        pr_fsm_next = PR_IDLE;
      end
      pr_fsm_reg[_PR_TO_RELEASE_]: begin
        if (ctrl_pr_fsm_fire) begin
          pr_fsm_next = PR_RELEASE;
          pr_fsm_key_next = code_data;
        end
      end
      pr_fsm_reg[_PR_RELEASE_]: begin
        pr_fsm_next = PR_IDLE;
      end
    endcase
  end

  always @(*) begin : pr_fsm_output
    code_ready <= 0;
    key_lshift_p <= 0;
    key_lshift_r <= 0;
    key_rshift_p <= 0;
    key_rshift_r <= 0;
    key_alt_p <= 0;
    key_alt_r <= 0;
    key_ctrl_p <= 0;
    key_ctrl_r <= 0;
    key_capslock_p <= 0;
    key_capslock_r <= 0;
    key_norm_d <= 0;
    key_norm_p <= 0;
    key_norm_r <= 0;

    case (1'b1)
      pr_fsm_reg[_PR_IDLE_]: begin
        code_ready <= 1;
      end
      pr_fsm_reg[_PR_PRESS_]: begin
        key_lshift_p <= pr_fsm_key_reg == 8'h12;
        key_rshift_p <= pr_fsm_key_reg == 8'h59;
        key_alt_p <= pr_fsm_key_reg == 8'h11;
        key_ctrl_p <= pr_fsm_key_reg == 8'h14;
        key_capslock_p <= pr_fsm_key_reg == 8'h58;
        key_norm_d <= pr_fsm_key_reg;
        key_norm_p <= ~|{key_lshift_p, key_rshift_p, key_alt_p, key_ctrl_p, key_capslock_p};
      end
      pr_fsm_reg[_PR_TO_RELEASE_]: begin
        code_ready <= 1;
      end
      pr_fsm_reg[_PR_RELEASE_]: begin
        key_lshift_r <= pr_fsm_key_reg == 8'h12;
        key_rshift_r <= pr_fsm_key_reg == 8'h59;
        key_alt_r <= pr_fsm_key_reg == 8'h11;
        key_ctrl_r <= pr_fsm_key_reg == 8'h14;
        key_norm_d <= pr_fsm_key_reg;
        key_norm_r <= ~|{key_lshift_r, key_rshift_r, key_alt_r, key_ctrl_r, key_capslock_r};
      end
    endcase
  end
endmodule
