module sCPU_fetcher (
    input  wire         clk_i,
    input  wire         rst_i,
    input  wire         pc_add_sel_i,
    input  wire [8-1:0] pc_add_val_i,
    output wire [8-1:0] pc_o
);

    reg [8-1:0] pc_val;
    always @(posedge rst_i or posedge clk_i) begin
        if (rst_i) begin
            pc_val <= 0;
        end else begin
            pc_val <= pc_val + (pc_add_sel_i ? pc_add_val_i : 8'd1);
        end
    end

    assign pc_o = pc_val;

endmodule
