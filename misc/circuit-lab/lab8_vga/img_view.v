module img_view (
    input  wire          clk_i,
    input  wire          rst_ni,
    input  wire          v_sync_rise_i,
    input  wire [10-1:0] v_addr_i,
    input  wire [10-1:0] h_addr_i,
    output wire [24-1:0] vga_data_o
);
    localparam WinWidth = 640;
    localparam WinHeight = 480;
    localparam ImgWidth = 64;
    localparam ImgHeight = 128;

    reg [23:0] vga_mem[ImgHeight*ImgWidth-1:0];
    initial begin
        $readmemh("resources/cute.hex", vga_mem);
    end

    // random source
    wire [5-1:0] x_speed_init;
    wire [5-1:0] y_speed_init;

    lfsr_prbs_gen #(
        .LFSR_WIDTH(16),
        .LFSR_INIT (16'h66cc),
        .DATA_WIDTH(5)
    ) x_speed_rand (
        .clk     (clk_i),
        .rst     (1'b0),
        .enable  (1'b1),
        .data_out(x_speed_init)
    );
    lfsr_prbs_gen #(
        .LFSR_WIDTH(16),
        .LFSR_INIT (16'hccff),
        .DATA_WIDTH(5)
    ) y_speed_rand (
        .clk     (clk_i),
        .rst     (1'b0),
        .enable  (1'b1),
        .data_out(y_speed_init)
    );

    reg [8-1:0] frame_cnt;
    always @(negedge rst_ni or posedge clk_i) begin
        if (!rst_ni) frame_cnt <= 8'd0;
        else if (v_sync_rise_i) frame_cnt <= frame_cnt + 8'd1;
    end

    // calculate state when v_sync is high
    reg v_sync_rise_1d;
    reg v_sync_rise_2d;
    always @(posedge clk_i) begin
        v_sync_rise_1d <= v_sync_rise_i;
        v_sync_rise_2d <= v_sync_rise_1d;
    end

    // speed is pixel per frame
    reg signed [ 5-1:0] x_speed;
    reg signed [ 5-1:0] y_speed;
    // position of left up and right down corner point
    reg signed [11-1:0] x_left;
    reg signed [11-1:0] y_up;
    reg signed [11-1:0] x_right;
    reg signed [11-1:0] y_down;
    always @(negedge rst_ni or posedge clk_i) begin
        if (!rst_ni) begin
            y_up    <= (WinHeight - ImgHeight) / 2;
            x_left  <= (WinWidth - ImgWidth) / 2;
            y_down  <= (WinHeight + ImgHeight) / 2;
            x_right <= (WinWidth + ImgWidth) / 2;
            y_speed <= y_speed_init;
            x_speed <= x_speed_init;
        end else if (v_sync_rise_i) begin
            // compute the new left up corner point
            x_left <= x_left + $signed({{6{x_speed[5-1]}}, x_speed});
            y_up   <= y_up + $signed({{6{y_speed[5-1]}}, y_speed});
        end else if (v_sync_rise_1d) begin
            // update speed and fix the position
            if (x_left < 0 || x_left + ImgWidth >= WinWidth) begin
                x_speed <= -x_speed;
            end
            if (y_up < 0 || y_up + ImgHeight >= WinHeight) begin
                y_speed <= -y_speed;
            end
            if (x_left < 0) begin
                x_left <= -x_left;
            end else if (x_left + ImgWidth >= WinWidth) begin
                x_left <= x_left - 2 * (x_left + ImgWidth - WinWidth);
            end
            if (y_up < 0) begin
                y_up <= -y_up;
            end else if (y_up + ImgHeight >= WinHeight) begin
                y_up <= y_up - 2 * (y_up + ImgHeight - WinHeight);
            end
        end else if (v_sync_rise_2d) begin
            // update the right down corner point
            y_down  <= y_up + ImgHeight;
            x_right <= x_left + ImgWidth;
        end
    end

    // draw img when v_sync and h_sync is low
    // y_up/down and x_left/right now must be in range
    wire [10-1:0] x_left_abs = x_left[10-1:0];
    wire [10-1:0] y_up_abs = y_up[10-1:0];
    wire [10-1:0] x_right_abs = x_right[10-1:0];
    wire [10-1:0] y_down_abs = y_down[10-1:0];
    wire          _x_right_unused = x_right[11-1];
    wire          _y_down_unused = y_down[11-1];

    // index of the img mem
    reg  [10-1:0] x_idx;
    reg  [10-1:0] y_idx;
    always @(posedge clk_i) begin
        x_idx <= h_addr_i - x_left_abs;
        y_idx <= v_addr_i - y_up_abs;
    end
    wire [ 4-1:0] _x_idx_unused = x_idx[10-1:6];
    wire [ 3-1:0] _y_idx_unused = y_idx[10-1:7];

    reg  [10-1:0] v_addr_1d;
    reg  [10-1:0] h_addr_1d;
    always @(posedge clk_i) begin
        v_addr_1d <= v_addr_i;
        h_addr_1d <= h_addr_i;
    end

    reg [24-1:0] vga_data_2d;
    always @(posedge clk_i) begin
        if (y_up_abs <= v_addr_1d && v_addr_1d < y_down_abs &&
            x_left_abs <= h_addr_1d && h_addr_1d < x_right_abs) begin
            vga_data_2d <= vga_mem[{y_idx[7-1:0], x_idx[6-1:0]}];
        end else begin
            vga_data_2d <= 24'hffffff;
        end
    end

    assign vga_data_o = vga_data_2d;

endmodule
