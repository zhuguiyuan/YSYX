module ps2_keyboard_driver (
    input  wire         clk_i,
    input  wire         rst_ni,
    input  wire         ps2_clk_i,
    input  wire         ps2_data_i,
    output wire         ps_lost_o,
    output wire         code_valid_o,
    output wire         code_down_o,
    output wire         code_up_o,
    output wire         code_extend_o,
    output wire [8-1:0] code_value_o,
    output wire         ctrl_valid_o,
    output wire         ctrl_lshift_o,
    output wire         ctrl_rshift_o,
    output wire         ctrl_lalt_o,
    output wire         ctrl_ralt_o,
    output wire         ctrl_lctrl_o,
    output wire         ctrl_rctrl_o,
    output wire         ctrl_capslock_o
);

    wire [8-1:0] code_value_0d;
    wire         code_valid_0d;
    ps2_keyboard u_ps2_keyboard (
        .clk(clk_i),
        .clrn(rst_ni),
        .ps2_clk(ps2_clk_i),
        .ps2_data(ps2_data_i),
        .overflow(ps_lost_o),
        .data(code_value_0d),
        .nextdata_n(1'b0),
        .ready(code_valid_0d)
    );

    // idle -> idle                               基本按键按下
    //      -> release -> idle                    基本按键释放
    //      -> extend  -> idle                    扩展按键按下
    //                 -> release extend -> idle  扩展按键释放

    localparam ST_IDLE = 4'b0001;
    localparam ST_EXTEND = 4'b0010;
    localparam ST_RELEASE = 4'b0100;
    localparam ST_RELEASE_EXTEND = 4'b1000;

    reg  [4-1:0] state_reg;
    wire [4-1:0] state_nxt;
    always @(posedge clk_i) begin
        if (!rst_ni) begin
            state_reg <= ST_IDLE;
        end else begin
            state_reg <= state_nxt;
        end
    end
    wire code_extend_prefix_0d = code_valid_0d && code_value_0d == 8'he0;
    wire code_release_prefix_0d = code_valid_0d && code_value_0d == 8'hf0;
    wire code_basic_0d = code_valid_0d && code_value_0d != 8'he0 && code_value_0d != 8'hf0;
    assign state_nxt =
        state_reg == ST_IDLE   && code_extend_prefix_0d  ? ST_EXTEND :
        state_reg == ST_IDLE   && code_release_prefix_0d ? ST_RELEASE :
        state_reg == ST_EXTEND && code_release_prefix_0d ? ST_RELEASE_EXTEND :
        code_basic_0d                                    ? ST_IDLE : state_reg;

    reg code_extend_0d;  // 相对最终数据的 0d
    always @(posedge clk_i) begin
        if (code_extend_prefix_0d) begin
            code_extend_0d <= 1'b1;
        end else if (state_reg == ST_IDLE) begin
            code_extend_0d <= 1'b0;
        end
    end

    wire pressing_evt_0d = (state_reg == ST_IDLE || state_reg == ST_EXTEND) && code_basic_0d;
    wire releasing_evt_0d = (state_reg == ST_RELEASE || state_reg == ST_RELEASE_EXTEND) && code_basic_0d;

    wire lshift_evt_0d = code_value_0d == 8'h12;
    wire rshift_evt_0d = code_value_0d == 8'h59;
    wire lalt_evt_0d = code_value_0d == 8'h11 && !code_extend_0d;
    wire ralt_evt_0d = code_value_0d == 8'h11 && code_extend_0d;
    wire lctrl_evt_0d = code_value_0d == 8'h14 && !code_extend_0d;
    wire rctrl_evt_0d = code_value_0d == 8'h14 && code_extend_0d;
    wire capslock_evt_0d = code_value_0d == 8'h58 && pressing_evt_0d;

    wire ctrl_key_evt_0d = |{lshift_evt_0d,rshift_evt_0d,lalt_evt_0d,ralt_evt_0d,
                             lctrl_evt_0d,rctrl_evt_0d,capslock_evt_0d};

    wire is_coding_control = code_valid_0d && ctrl_key_evt_0d;
    wire is_coding_normal = code_valid_0d && ~ctrl_key_evt_0d;

    // 普通按键的状态处理
    reg is_pressing_lock;
    reg code_valid_1d;
    reg code_down_1d;
    reg code_up_1d;
    reg code_extend_1d;
    reg [8-1:0] code_value_1d;
    wire code_new_eq_old = code_value_0d == code_value_1d && code_extend_0d == code_extend_1d;
    always @(posedge clk_i) begin
        // 默认没有按键事件
        code_valid_1d <= 1'b0;
        if (!rst_ni) begin
            is_pressing_lock <= 1'd0;
            code_valid_1d    <= 1'b0;
            code_down_1d     <= 1'b0;
            code_up_1d       <= 1'b0;
            code_extend_1d   <= 1'b0;
        end else if (is_pressing_lock) begin
            // 当前已经按下了某个普通键
            if (pressing_evt_0d && is_coding_normal && code_new_eq_old) begin
                // 新按下的按键和原来相同，生成一个事件，但不是按下也不是释放
                code_valid_1d <= 1'b1;
                code_down_1d  <= 1'b0;
                code_up_1d    <= 1'b0;
            end else if (releasing_evt_0d && is_coding_normal && code_new_eq_old) begin
                // 释放了按下的按键，生成一个释放事件
                code_valid_1d <= 1'b1;
                code_down_1d <= 1'b0;
                code_up_1d <= 1'b1;
                is_pressing_lock <= 1'b0;
            end
        end else if (!is_pressing_lock) begin
            // 当前没有按下某个普通按键
            if (pressing_evt_0d && is_coding_normal) begin
                // 按下了一个按键，生成一个按下事件
                code_valid_1d <= 1'b1;
                code_down_1d <= 1'b1;
                code_up_1d <= 1'b0;
                code_value_1d <= code_value_0d;
                code_extend_1d <= code_extend_0d;
                is_pressing_lock <= 1'b1;
            end
        end else begin
            // cannot reach here
        end
    end
    assign code_valid_o = code_valid_1d;
    assign code_down_o = code_down_1d;
    assign code_up_o = code_up_1d;
    assign code_extend_o = code_extend_1d;
    assign code_value_o = code_value_1d;

    // 控制按键的状态处理
    reg ctrl_valid_1d;
    reg ctrl_lshift_1d;
    reg ctrl_rshift_1d;
    reg ctrl_lalt_1d;
    reg ctrl_ralt_1d;
    reg ctrl_lctrl_1d;
    reg ctrl_rctrl_1d;
    reg ctrl_capslock_1d;
    always @(posedge clk_i) begin
        if (!rst_ni) begin
            ctrl_valid_1d <= 1'b0;
            ctrl_lshift_1d <= 1'b0;
            ctrl_rshift_1d <= 1'b0;
            ctrl_lalt_1d <= 1'b0;
            ctrl_ralt_1d <= 1'b0;
            ctrl_lctrl_1d <= 1'b0;
            ctrl_rctrl_1d <= 1'b0;
            ctrl_capslock_1d <= 1'b0;
        end else if (is_coding_control) begin
            ctrl_valid_1d <= 1'b1;
            if (lshift_evt_0d) ctrl_lshift_1d <= pressing_evt_0d;
            if (rshift_evt_0d) ctrl_rshift_1d <= pressing_evt_0d;
            if (lalt_evt_0d) ctrl_lalt_1d <= pressing_evt_0d;
            if (ralt_evt_0d) ctrl_ralt_1d <= pressing_evt_0d;
            if (lctrl_evt_0d) ctrl_lctrl_1d <= pressing_evt_0d;
            if (rctrl_evt_0d) ctrl_rctrl_1d <= pressing_evt_0d;
            if (capslock_evt_0d) ctrl_capslock_1d <= ~ctrl_capslock_1d;
        end else begin
            ctrl_valid_1d <= 1'b0;
        end
    end

    assign ctrl_valid_o = ctrl_valid_1d;
    assign ctrl_lshift_o = ctrl_lshift_1d;
    assign ctrl_rshift_o = ctrl_rshift_1d;
    assign ctrl_lalt_o = ctrl_lalt_1d;
    assign ctrl_ralt_o = ctrl_ralt_1d;
    assign ctrl_lctrl_o = ctrl_lctrl_1d;
    assign ctrl_rctrl_o = ctrl_rctrl_1d;
    assign ctrl_capslock_o = ctrl_capslock_1d;

endmodule
