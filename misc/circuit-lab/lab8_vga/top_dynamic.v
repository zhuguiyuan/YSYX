module top_dynamic (
    input  wire       clk_i,
    input  wire       rst_ni,
    output wire       VGA_CLK_o,
    output wire       VGA_HSYNC_o,
    output wire       VGA_VSYNC_o,
    output wire       VGA_BLANK_N_o,
    output wire [7:0] VGA_R_o,
    output wire [7:0] VGA_G_o,
    output wire [7:0] VGA_B_o
);

    assign VGA_CLK_o = clk_i;

    wire [10-1:0] h_addr_0d;
    wire [10-1:0] v_addr_0d;
    wire          h_sync_0d;
    wire          v_sync_0d;
    wire          blank_n_0d;

    localparam DATA_DELAY_CYCLE = 2;
    wire          h_sync_Nd;
    wire          v_sync_Nd;
    wire          blank_n_Nd;
    wire [24-1:0] vga_data_Nd;
    wire [ 8-1:0] vga_r_Nd;
    wire [ 8-1:0] vga_g_Nd;
    wire [ 8-1:0] vga_b_Nd;
    vga_ctrl my_vga_ctrl (
        .pclk    (clk_i),
        .reset   (~rst_ni),
        .vga_data(vga_data_Nd),
        .h_addr  (h_addr_0d),
        .v_addr  (v_addr_0d),
        .hsync   (h_sync_0d),
        .vsync   (v_sync_0d),
        .valid   (blank_n_0d),
        .vga_r   (vga_r_Nd),
        .vga_g   (vga_g_Nd),
        .vga_b   (vga_b_Nd)
    );
    bits_slr #(
        .DEPTH(DATA_DELAY_CYCLE),
        .WIDTH(3)
    ) u_slr (
        .clk_i(clk_i),
        .d_i  ({h_sync_0d, v_sync_0d, blank_n_0d}),
        .q_o  ({h_sync_Nd, v_sync_Nd, blank_n_Nd})
    );

    reg v_sync_1d;
    always @(posedge clk_i) v_sync_1d <= v_sync_0d;
    wire v_sync_rise_0d = v_sync_0d & ~v_sync_1d;

    img_view u_img_view (
        .clk_i        (clk_i),
        .rst_ni       (rst_ni),
        .v_sync_rise_i(v_sync_rise_0d),
        .v_addr_i     (v_addr_0d),
        .h_addr_i     (h_addr_0d),
        .vga_data_o   (vga_data_Nd)
    );

    assign VGA_HSYNC_o = h_sync_Nd;
    assign VGA_VSYNC_o = v_sync_Nd;
    assign VGA_BLANK_N_o = blank_n_Nd;
    assign VGA_R_o = vga_r_Nd;
    assign VGA_G_o = vga_g_Nd;
    assign VGA_B_o = vga_b_Nd;

endmodule

