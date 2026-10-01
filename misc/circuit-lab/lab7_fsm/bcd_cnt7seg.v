module bcd_cnt7seg (
    input  wire         clk_i,
    input  wire         rst_ni,
    input  wire         code_press_i,
    output wire [8-1:0] bcd_cnt_lo_o,
    output wire [8-1:0] bcd_cnt_hi_o
);

    reg [8-1:0] code_press_cnt;
    always @(posedge clk_i) begin
        if (!rst_ni) begin
            code_press_cnt <= 0;
        end else if (code_press_i) begin
            code_press_cnt <= code_press_cnt + 1;
        end
    end

    rom7seg u_rom7seg_lo (
        .data_i(code_press_cnt[4-1:0]),
        .data_o(bcd_cnt_lo_o)
    );
    rom7seg u_rom7seg_hi (
        .data_i(code_press_cnt[8-1:4]),
        .data_o(bcd_cnt_hi_o)
    );

endmodule
