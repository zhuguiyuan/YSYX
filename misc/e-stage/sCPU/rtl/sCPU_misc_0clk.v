module sCPU_misc_0clk (
    input  wire         is_add_i,
    input  wire         is_li_i,
    input  wire         is_io_i,
    input  wire         is_bner0_i,
    input  wire         io_in_i,
    input  wire [2-1:0] rd_i,
    input  wire [2-1:0] rs1_i,
    input  wire [2-1:0] rs2_i,
    input  wire [8-1:0] imm_i,
    input  wire [8-1:0] io_in_val_i,
    output wire [2-1:0] gpr_rd_addr0_o,
    input  wire [8-1:0] gpr_rd_data0_i,
    output wire [2-1:0] gpr_rd_addr1_o,
    input  wire [8-1:0] gpr_rd_data1_i,
    output wire         gpr_wr_en_o,
    output wire [2-1:0] gpr_wr_addr_o,
    output wire [8-1:0] gpr_wr_data_o,
    output wire         ner0_o
);
    wire [8-1:0] sum_result = gpr_rd_data0_i + gpr_rd_data1_i;

    assign gpr_rd_addr0_o = is_bner0_i ? 2'd0 : rs1_i;
    assign gpr_rd_addr1_o = is_io_i ? rd_i : rs2_i;
    assign gpr_wr_en_o = is_add_i | is_li_i | io_in_i;
    assign gpr_wr_data_o = is_li_i ? imm_i : io_in_i ? io_in_val_i : sum_result;
    assign gpr_wr_addr_o = rd_i;

    assign ner0_o = gpr_rd_data0_i != gpr_rd_data1_i;

endmodule
