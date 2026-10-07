module sCPU_gpr (
    input  wire         clk_i,
    input  wire         rst_i,
    input  wire [2-1:0] rd_addr_0_i,
    output wire [8-1:0] rd_data_0_o,
    input  wire [2-1:0] rd_addr_1_i,
    output wire [8-1:0] rd_data_1_o,
    input  wire         wr_en_i,
    input  wire [2-1:0] wr_addr_i,
    input  wire [8-1:0] wr_data_i
);

    reg [8-1:0] reg_file[0:4-1];
    initial begin
        integer i;
        for (i = 0; i < 4; i = i + 1) begin
            reg_file[i] = 8'd0;
        end
    end

    // rst_i has been synced to clk_i
    wire wr_en_gated = wr_en_i & ~rst_i;
    always @(posedge clk_i) begin
        if (wr_en_gated) begin
            reg_file[wr_addr_i] <= wr_data_i;
        end
    end

    assign rd_data_0_o = reg_file[rd_addr_0_i];
    assign rd_data_1_o = reg_file[rd_addr_1_i];

endmodule
