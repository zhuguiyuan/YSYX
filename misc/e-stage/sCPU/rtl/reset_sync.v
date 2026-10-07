module reset_sync (
    input  wire clk_i,
    input  wire rst_async_ni,
    output wire rst_sync_o,
    output wire rst_done_o
);

    reg [2-1:0] rst_sync_reg;

    always @(posedge clk_i or negedge rst_async_ni) begin
        if (!rst_async_ni) begin
            rst_sync_reg <= 2'b11;
        end else begin
            rst_sync_reg <= {rst_sync_reg[0], 1'b0};
        end
    end

    assign rst_sync_o = rst_sync_reg[2-1];
    assign rst_done_o = rst_sync_o;

endmodule
