module ps2_system (
    input  wire       clk,
    input  wire       rstn,
    input  wire       ps2_clk,
    input  wire       ps2_data,
    output wire       ps2_error,
    output wire       key_capslock_s,
    output wire       key_lshift_s,
    output wire       key_rshift_s,
    output wire       key_alt_s,
    output wire       key_ctrl_s,
    output wire       key_norm_s,
    output wire [7:0] key_norm_code
);

  wire [7:0] code_data;
  wire       code_valid;
  wire       code_ready;
  ps2_keyboard_wrapper u_ps2_keyboard_wrapper (
    .clk        (clk       ),
    .rstn       (rstn      ),
    .ps2_clk    (ps2_clk   ),
    .ps2_data   (ps2_data  ),
    .code_data  (code_data ),
    .code_valid (code_valid),
    .code_ready (code_ready),
    .code_error (ps2_error )
  );

  wire       key_lshift_p;
  wire       key_lshift_r;
  wire       key_rshift_p;
  wire       key_rshift_r;
  wire       key_alt_p;
  wire       key_alt_r;
  wire       key_ctrl_p;
  wire       key_ctrl_r;
  wire       key_capslock_p;
  /* verilator lint_off UNUSEDSIGNAL */
  wire       key_capslock_r;
  wire [7:0] key_norm_d;
  wire       key_norm_p;
  wire       key_norm_r;
  ps2_master_fsm u_master_fsm (
    .clk            (clk           ),
    .rstn           (rstn          ),
    .code_data      (code_data     ),
    .code_valid     (code_valid    ),
    .code_ready     (code_ready    ),
    .key_lshift_p   (key_lshift_p  ),
    .key_lshift_r   (key_lshift_r  ),
    .key_rshift_p   (key_rshift_p  ),
    .key_rshift_r   (key_rshift_r  ),
    .key_alt_p      (key_alt_p     ),
    .key_alt_r      (key_alt_r     ),
    .key_ctrl_p     (key_ctrl_p    ),
    .key_ctrl_r     (key_ctrl_r    ),
    .key_capslock_p (key_capslock_p),
    .key_capslock_r (key_capslock_r),
    .key_norm_d     (key_norm_d    ),
    .key_norm_p     (key_norm_p    ),
    .key_norm_r     (key_norm_r    )
  );

  ps2_capslock_fsm u_capslock_fsm (
    .clk            (clk           ),
    .rstn           (rstn          ),
    .key_capslock_p (key_capslock_p),
    .capslock_s     (key_capslock_s)
  );

  ps2_control_key_fsm u_lshift_fsm (
    .clk   (clk         ),
    .rstn  (rstn        ),
    .key_p (key_lshift_p),
    .key_r (key_lshift_r),
    .key_s (key_lshift_s)
  );

  ps2_control_key_fsm u_rshift_fsm (
    .clk   (clk         ),
    .rstn  (rstn        ),
    .key_p (key_rshift_p),
    .key_r (key_rshift_r),
    .key_s (key_rshift_s)
  );

  ps2_control_key_fsm u_alt_fsm (
    .clk   (clk      ),
    .rstn  (rstn     ),
    .key_p (key_alt_p),
    .key_r (key_alt_r),
    .key_s (key_alt_s)
  );

  ps2_control_key_fsm u_ctrl_fsm (
    .clk   (clk       ),
    .rstn  (rstn      ),
    .key_p (key_ctrl_p),
    .key_r (key_ctrl_r),
    .key_s (key_ctrl_s)
  );

  ps2_normal_key_fsm u_norm_fsm (
    .clk   (clk           ),
    .rstn  (rstn          ),
    .key_d (key_norm_d    ),
    .key_p (key_norm_p    ),
    .key_r (key_norm_r    ),
    .key_s (key_norm_s    ),
    .code  (key_norm_code )
  );

endmodule
