module top (
    input wire clk_i,
    input wire rst_ni,

    input wire ps2_clk_i,
    input wire ps2_data_i,

    output wire [8-1:0] bcd_raw_lo_o,
    output wire [8-1:0] bcd_raw_hi_o,
    output wire         bcd_raw_ext_o,
    output wire [8-1:0] bcd_ascii_lo_o,
    output wire [8-1:0] bcd_ascii_hi_o,
    output wire [8-1:0] bcd_cnt_lo_o,
    output wire [8-1:0] bcd_cnt_hi_o,

    output wire led_lshift_o,
    output wire led_rshift_o,
    output wire led_lalt_o,
    output wire led_ralt_o,
    output wire led_lctrl_o,
    output wire led_rctrl_o,
    output wire led_capslock_o,

    output wire led_ps2_lost_o
);

    wire         code_valid;
    wire         code_down;
    wire         code_up;
    wire         code_extend;
    wire [8-1:0] code_value;
    wire         ctrl_valid;
    wire         ctrl_lshift;
    wire         ctrl_rshift;
    wire         ctrl_lalt;
    wire         ctrl_ralt;
    wire         ctrl_lctrl;
    wire         ctrl_rctrl;
    wire         ctrl_capslock;
    ps2_keyboard_driver u_ps2_keyboard_driver (
        .clk_i          (clk_i),
        .rst_ni         (rst_ni),
        .ps2_clk_i      (ps2_clk_i),
        .ps2_data_i     (ps2_data_i),
        .ps_lost_o      (led_ps2_lost_o),
        .code_valid_o   (code_valid),
        .code_down_o    (code_down),
        .code_up_o      (code_up),
        .code_extend_o  (code_extend),
        .code_value_o   (code_value),
        .ctrl_valid_o   (ctrl_valid),
        .ctrl_lshift_o  (ctrl_lshift),
        .ctrl_rshift_o  (ctrl_rshift),
        .ctrl_lalt_o    (ctrl_lalt),
        .ctrl_ralt_o    (ctrl_ralt),
        .ctrl_lctrl_o   (ctrl_lctrl),
        .ctrl_rctrl_o   (ctrl_rctrl),
        .ctrl_capslock_o(ctrl_capslock)
    );

    // raw code
    bcd_raw7seg u_raw7seg (
        .clk_i        (clk_i),
        .rst_ni       (rst_ni),
        .code_valid_i (code_valid),
        .code_down_i  (code_down),
        .code_up_i    (code_up),
        .code_extend_i(code_extend),
        .code_value_i (code_value),
        .bcd_raw_lo_o (bcd_raw_lo_o),
        .bcd_raw_hi_o (bcd_raw_hi_o),
        .bcd_raw_ext_o(bcd_raw_ext_o)
    );

    // ascii code
    bcd_ascii7seg u_ascii7seg (
        .clk_i          (clk_i),
        .rst_ni         (rst_ni),
        .code_valid_i   (code_valid),
        .code_down_i    (code_down),
        .code_up_i      (code_up),
        .code_extend_i  (code_extend),
        .code_value_i   (code_value),
        .ctrl_lshift_i  (ctrl_lshift),
        .ctrl_rshift_i  (ctrl_rshift),
        .ctrl_capslock_i(ctrl_capslock),
        .bcd_ascii_lo_o (bcd_ascii_lo_o),
        .bcd_ascii_hi_o (bcd_ascii_hi_o)
    );

    // press count
    bcd_cnt7seg u_cnt7seg (
        .clk_i       (clk_i),
        .rst_ni      (rst_ni),
        .code_press_i(code_valid && code_down),
        .bcd_cnt_lo_o(bcd_cnt_lo_o),
        .bcd_cnt_hi_o(bcd_cnt_hi_o)
    );

    // leds
    reg ctrl_lshift_reg;
    reg ctrl_rshift_reg;
    reg ctrl_lalt_reg;
    reg ctrl_ralt_reg;
    reg ctrl_lctrl_reg;
    reg ctrl_rctrl_reg;
    reg ctrl_capslock_reg;
    always @(posedge clk_i) begin
        if (!rst_ni) begin
            ctrl_lshift_reg   <= 1'b0;
            ctrl_rshift_reg   <= 1'b0;
            ctrl_lalt_reg     <= 1'b0;
            ctrl_ralt_reg     <= 1'b0;
            ctrl_lctrl_reg    <= 1'b0;
            ctrl_rctrl_reg    <= 1'b0;
            ctrl_capslock_reg <= 1'b0;
        end else if (ctrl_valid) begin
            ctrl_lshift_reg   <= ctrl_lshift;
            ctrl_rshift_reg   <= ctrl_rshift;
            ctrl_lalt_reg     <= ctrl_lalt;
            ctrl_ralt_reg     <= ctrl_ralt;
            ctrl_lctrl_reg    <= ctrl_lctrl;
            ctrl_rctrl_reg    <= ctrl_rctrl;
            ctrl_capslock_reg <= ctrl_capslock;
        end
    end
    assign led_lshift_o   = ctrl_lshift_reg;
    assign led_rshift_o   = ctrl_rshift_reg;
    assign led_lalt_o     = ctrl_lalt_reg;
    assign led_ralt_o     = ctrl_ralt_reg;
    assign led_lctrl_o    = ctrl_lctrl_reg;
    assign led_rctrl_o    = ctrl_rctrl_reg;
    assign led_capslock_o = ctrl_capslock_reg;

endmodule
