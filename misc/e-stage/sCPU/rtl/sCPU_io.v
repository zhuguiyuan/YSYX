module sCPU_io (
    input  wire         clk_i,
    input  wire         rst_i,
    input  wire         io_out_i,
    input  wire [3-1:0] dev_i,
    input  wire [8-1:0] out_val_i,
    output wire [8-1:0] odev0_o,
    output wire [8-1:0] odev1_o,
    input  wire [8-1:0] idev0_i,
    input  wire [8-1:0] idev1_i,
    output wire [8-1:0] io_in_val_o
);
    reg [8-1:0] idev0_sync;
    reg [8-1:0] idev1_sync;
    always @(posedge clk_i) begin
        idev0_sync <= idev0_i;
        idev1_sync <= idev1_i;
    end

    reg [8-1:0] odev0_reg, odev1_reg;
    always @(posedge clk_i or posedge rst_i) begin
        if (rst_i) begin
            odev0_reg <= 8'd0;
            odev1_reg <= 8'd0;
        end else if (io_out_i) begin
            if (dev_i == 3'd0) odev0_reg <= out_val_i;
            if (dev_i == 3'd1) odev1_reg <= out_val_i;
        end
    end
    assign odev0_o = odev0_reg;
    assign odev1_o = odev1_reg;
    assign io_in_val_o = dev_i == 3'd0 ? idev0_sync : dev_i == 3'd1 ? idev1_sync : 8'd0;

endmodule

