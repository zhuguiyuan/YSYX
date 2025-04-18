module top (
    input  wire       clk,
    input  wire       rstn,

    output wire [7:0] bcd_code_lo,
    output wire [7:0] bcd_code_hi,
    output wire [7:0] bcd_ascii_lo,
    output wire [7:0] bcd_ascii_hi,
    output wire [7:0] bcd_cnt_lo,
    output wire [7:0] bcd_cnt_hi,

    output wire       led_lshift,
    output wire       led_rshift,
    output wire       led_alt,
    output wire       led_ctrl,
    output wire       led_capslock,

    output wire       led_error
);

  // output declaration of module ps2_system
  wire       ps2_error;
  wire       key_capslock_s;
  wire       key_lshift_s;
  wire       key_rshift_s;
  wire       key_alt_s;
  wire       key_ctrl_s;
  wire       key_norm_s;
  wire [7:0] key_norm_code;

  ps2_system u_ps2_system (
    .clk            (clk           ),
    .rstn           (rstn          ),
    .ps2_clk        (ps2_clk       ),
    .ps2_data       (ps2_data      ),
    .ps2_error      (ps2_error     ),
    .key_capslock_s (key_capslock_s),
    .key_lshift_s   (key_lshift_s  ),
    .key_rshift_s   (key_rshift_s  ),
    .key_alt_s      (key_alt_s     ),
    .key_ctrl_s     (key_ctrl_s    ),
    .key_norm_s     (key_norm_s    ),
    .key_norm_code  (key_norm_code )
  );

  bcd7seg u_bcd_code_lo (
    .en  	(key_norm_s        ),
    .in  	(key_norm_code[3:0]),
    .out 	(bcd_ascii_lo      )
  );

  bcd7seg u_bcd_code_hi (
    .en  	(key_norm_s        ),
    .in  	(key_norm_code[7:4]),
    .out 	(bcd_ascii_hi      )
  );

  wire [7:0] key_norm_ascii = 0;
  bcd7seg u_bcd_ascii_lo (
    .en  	(key_norm_s         ),
    .in  	(key_norm_ascii[3:0]),
    .out 	(bcd_ascii_lo       )
  );

  bcd7seg u_bcd_ascii_hi (
    .en  	(key_norm_s         ),
    .in  	(key_norm_ascii[7:4]),
    .out 	(bcd_ascii_hi       )
  );

  wire [7:0] key_norm_cnt= 0;
  bcd7seg u_bcd_cnt_lo (
    .en  	(1'b1             ),
    .in  	(key_norm_cnt[3:0]),
    .out 	(bcd_cnt_lo       )
  );

  bcd7seg u_bcd_cnt_hi (
    .en  	(1'b1             ),
    .in  	(key_norm_cnt[7:4]),
    .out 	(bcd_cnt_hi       )
  );

  assign led_alt      = key_alt_s;
  assign led_ctrl     = key_ctrl_s;
  assign led_capslock = key_capslock_s;
  assign led_lshift   = key_lshift_s;
  assign led_rshift   = key_rshift_s;
  assign led_error    = ps2_error;

endmodule