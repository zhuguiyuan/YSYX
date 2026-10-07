module sCPU_decoder_0clk (
    input  wire [8-1:0] inst_i,
    output wire         is_add_o,
    output wire         is_io_o,
    output wire         is_li_o,
    output wire         is_bner0_o,
    output wire [2-1:0] rd_o,
    output wire [2-1:0] rs1_o,
    output wire [2-1:0] rs2_o,
    output wire [8-1:0] imm_o,
    output wire [8-1:0] addr_o,
    output wire         io_in_o,
    output wire         io_out_o,
    output wire [3-1:0] io_dev_o
);

    //   7  6 5  4 3   2 1   0
    //  +----+----+-----+-----+
    //  | 00 | rd | rs1 | rs2 | R[rd]=R[rs1]+R[rs2]
    //  +----+----+---+-+-----+
    //  | 01 | rd |i/o|  idx  | R[rd]<=>dev[idx]
    //  +----+----+---+-+-----+
    //  | 10 | rd |  s  | imm | R[rd]=imm << (s << 1)
    //  +----+----+-----+-----+
    //  | 11 |  offset  | rs2 | if (R[0]!=R[rs2]) PC=PC+sign_ext(offset)
    //  +----+----------+-----+
    wire [2-1:0] inst_op = inst_i[7:6];
    wire [2-1:0] inst_rd = inst_i[5:4];
    wire [2-1:0] inst_rs1 = inst_i[3:2];
    wire [2-1:0] inst_rs2 = inst_i[1:0];
    wire [1-1:0] inst_io = inst_i[3:3];
    wire [3-1:0] inst_idx = inst_i[2:0];
    wire [2-1:0] inst_s = inst_i[3:2];
    wire [2-1:0] inst_imm = inst_i[1:0];
    wire [4-1:0] inst_offset = inst_i[5:2];

    assign is_add_o = inst_op == 2'b00;
    assign is_io_o = inst_op == 2'b01;
    assign is_li_o = inst_op == 2'b10;
    assign is_bner0_o = inst_op == 2'b11;
    assign rd_o = inst_rd;
    assign rs1_o = inst_rs1;
    assign rs2_o = inst_rs2;
    assign imm_o = {6'b0, inst_imm} << (inst_s << 1);
    assign addr_o = {{4{inst_offset[4-1]}}, inst_offset[4-1:0]};
    assign io_in_o = is_io_o & ~inst_io;
    assign io_out_o = is_io_o & inst_io;
    assign io_dev_o = inst_idx;

endmodule
