module ps2_keyboard_wrapper(
    // clk and resetn
    input  wire clk,
    input  wire resetn,
    // ps2 interface
    input  wire ps2_clk,
    input  wire ps2_data,
    // code interface
    output wire code_data,
    output wire code_valid,
    input  wire code_ready,
    output wire code_error
  );

  ps2_keyboard u_ps2_keyboard (
    .clk        (clk        ),
    .clrn       (resetn     ),
    .ps2_clk    (ps2_clk    ),
    .ps2_data   (ps2_data   ),
    .data       (code_data  ),
    .ready      (code_valid ),
    .nextdata_n (~code_ready),
    .overflow   (code_error )
  );

endmodule
