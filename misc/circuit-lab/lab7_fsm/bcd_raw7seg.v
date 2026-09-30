module bcd_raw7seg (
    input  wire         clk_i,
    input  wire         rst_ni,
    input  wire         code_valid_i,
    input  wire         code_down_i,
    input  wire         code_up_i,
    input  wire         code_extend_i,
    input  wire [8-1:0] code_value_i,
    output wire [8-1:0] bcd_raw_lo_o,
    output wire [8-1:0] bcd_raw_hi_o,
    output wire         bcd_raw_ext_o
);

    wire [16-1:0] code_raw_imm;
    rom7seg u_rom7seg_lo (
        .data_i(code_value_i[4-1:0]),
        .data_o(code_raw_imm[8-1:0])
    );
    rom7seg u_rom7seg_hi (
        .data_i(code_value_i[8-1:4]),
        .data_o(code_raw_imm[16-1:8])
    );

    reg [16-1:0] code_raw_reg;
    always @(posedge clk_i) begin
        if (!rst_ni) begin
            code_raw_reg <= 16'hffff;
        end else if (code_valid_i && code_down_i) begin
            code_raw_reg <= code_raw_imm;
        end else if (code_valid_i && code_up_i) begin
            code_raw_reg <= 16'hffff;
        end
    end

    reg code_ext_reg;
    always @(posedge clk_i) begin
        if (!rst_ni) begin
            code_ext_reg <= 1'b0;
        end else if (code_valid_i && code_down_i) begin
            code_ext_reg <= code_extend_i;
        end else if (code_valid_i && code_up_i) begin
            code_ext_reg <= 1'b0;
        end
    end

    assign {bcd_raw_hi_o, bcd_raw_lo_o} = code_raw_reg;
    assign bcd_raw_ext_o = code_ext_reg;

endmodule
