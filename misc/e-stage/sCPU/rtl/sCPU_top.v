module sCPU_top (
    input wire clk_i,
    input wire rst_ni,
    output wire [8-1:0] seg_lo_o,
    output wire [8-1:0] seg_hi_o,
    output wire [8-1:0] led_o
);

    wire [8-1:0] pc;
    wire [8-1:0] inst;
    sCPU_rom u_rom (
        .pc_i  (pc),
        .inst_o(inst)
    );

    wire [8-1:0] odev0;
    wire [8-1:0] odev1;
    sCPU_core u_core (
        .clk_i(clk_i),
        .rst_i(~rst_ni),
        .pc_o(pc),
        .inst_i(inst),
        .odev0_o(odev0),
        .odev1_o(odev1),
        .idev0_i(8'd0),
        .idev1_i(8'd0)
    );

    assign led_o = odev0;

    rom7seg u_seg_lo (
        .data_i(odev1[4-1:0]),
        .data_o(seg_lo_o)
    );

    rom7seg u_seg_hi (
        .data_i(odev1[8-1:4]),
        .data_o(seg_hi_o)
    );

`ifndef SYNTHESIS
    wire [8-1:0] r0 = u_core.u_grp.reg_file[0];
    wire [8-1:0] r1 = u_core.u_grp.reg_file[1];
    wire [8-1:0] r2 = u_core.u_grp.reg_file[2];
    wire [8-1:0] r3 = u_core.u_grp.reg_file[3];
    initial begin
        $monitor("Time=%0t | [MONITOR] regfile changed to: %d %d %d %d", $time, r0, r1, r2, r3);
    end
`endif

endmodule
