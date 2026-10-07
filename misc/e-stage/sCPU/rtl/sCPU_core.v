module sCPU_core (
    input  wire         clk_i,
    input  wire         rst_i,
    output wire [8-1:0] pc_o,
    input  wire [8-1:0] inst_i,
    output wire [8-1:0] odev0_o,
    output wire [8-1:0] odev1_o,
    input  wire [8-1:0] idev0_i,
    input  wire [8-1:0] idev1_i
);

    wire         is_add;
    wire         is_io;
    wire         is_li;
    wire         is_bner0;
    wire [2-1:0] rd;
    wire [2-1:0] rs1;
    wire [2-1:0] rs2;
    wire [8-1:0] imm;
    wire [8-1:0] addr;
    wire         io_in;
    wire         io_out;
    wire [3-1:0] io_dev;
    sCPU_decoder_0clk u_deocer (
        .inst_i(inst_i),
        .is_add_o(is_add),
        .is_io_o(is_io),
        .is_li_o(is_li),
        .is_bner0_o(is_bner0),
        .rd_o(rd),
        .rs1_o(rs1),
        .rs2_o(rs2),
        .imm_o(imm),
        .addr_o(addr),
        .io_in_o(io_in),
        .io_out_o(io_out),
        .io_dev_o(io_dev)
    );

    wire [8-1:0] io_in_val;
    wire [2-1:0] gpr_rd_addr0;
    wire [8-1:0] gpr_rd_data0;
    wire [2-1:0] gpr_rd_addr1;
    wire [8-1:0] gpr_rd_data1;
    wire         gpr_wr_en;
    wire [2-1:0] gpr_wr_addr;
    wire [8-1:0] gpr_wr_data;
    wire         ner0;
    sCPU_misc_0clk u_misc (
        .is_add_i(is_add),
        .is_li_i(is_li),
        .is_io_i(is_io),
        .is_bner0_i(is_bner0),
        .io_in_i(io_in),
        .rd_i(rd),
        .rs1_i(rs1),
        .rs2_i(rs2),
        .imm_i(imm),
        .io_in_val_i(io_in_val),
        .gpr_rd_addr0_o(gpr_rd_addr0),
        .gpr_rd_data0_i(gpr_rd_data0),
        .gpr_rd_addr1_o(gpr_rd_addr1),
        .gpr_rd_data1_i(gpr_rd_data1),
        .gpr_wr_en_o(gpr_wr_en),
        .gpr_wr_addr_o(gpr_wr_addr),
        .gpr_wr_data_o(gpr_wr_data),
        .ner0_o(ner0)

    );

    sCPU_gpr u_grp (
        .clk_i(clk_i),
        .rd_addr_0_i(gpr_rd_addr0),
        .rd_data_0_o(gpr_rd_data0),
        .rd_addr_1_i(gpr_rd_addr1),
        .rd_data_1_o(gpr_rd_data1),
        .wr_en_i(gpr_wr_en),
        .wr_addr_i(gpr_wr_addr),
        .wr_data_i(gpr_wr_data)
    );


    sCPU_fetcher u_fetcher (
        .clk_i(clk_i),
        .rst_i(rst_i),
        .pc_add_sel_i(is_bner0 && ner0),
        .pc_add_val_i(addr),
        .pc_o(pc_o)
    );

    sCPU_io u_io (
        .clk_i(clk_i),
        .rst_i(rst_i),
        .io_out_i(io_out),
        .dev_i(io_dev),
        .out_val_i(gpr_rd_data1),
        .odev0_o(odev0_o),
        .odev1_o(odev1_o),
        .idev0_i(idev0_i),
        .idev1_i(idev1_i),
        .io_in_val_o(io_in_val)
    );

endmodule
