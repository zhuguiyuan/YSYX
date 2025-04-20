module top (
    input  wire       clk,
    input  wire       rstn,

    input  wire       ps2_clk,
    input  wire       ps2_data,

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
    .out 	(bcd_code_lo       )
  );

  bcd7seg u_bcd_code_hi (
    .en  	(key_norm_s        ),
    .in  	(key_norm_code[7:4]),
    .out 	(bcd_code_hi       )
  );

  wire key_up_case = (key_lshift_s | key_rshift_s) ^ key_capslock_s;
  wire [7:0] key_norm_ascii;
  MuxKeyWithDefault #(
    .NR_KEY(102),
    .KEY_LEN(9),
    .DATA_LEN(8)
  ) u_MuxKeyWithDefault (
    .out         	(key_norm_ascii              ),
    .key         	({key_up_case, key_norm_code}),
    .default_out 	(8'h00                       ),
    .lut         	({
      17'b00000111001100000,
      17'b00001011000110001,
      17'b00001111000110010,
      17'b00010011000110011,
      17'b00010010100110100,
      17'b00010111000110101,
      17'b00011011000110110,
      17'b00011110100110111,
      17'b00011111000111000,
      17'b00100011000111001,
      17'b00100010100110000,
      17'b00100111000101101,
      17'b00101010100111101,
      17'b00001110001100001,
      17'b00011001001100010,
      17'b00010000101100011,
      17'b00010001101100100,
      17'b00010010001100101,
      17'b00010101101100110,
      17'b00011010001100111,
      17'b00011001101101000,
      17'b00100001101101001,
      17'b00011101101101010,
      17'b00100001001101011,
      17'b00100101101101100,
      17'b00011101001101101,
      17'b00011000101101110,
      17'b00100010001101111,
      17'b00100110101110000,
      17'b00001010101110001,
      17'b00010110101110010,
      17'b00001101101110011,
      17'b00010110001110100,
      17'b00011110001110101,
      17'b00010101001110110,
      17'b00001110101110111,
      17'b00010001001111000,
      17'b00011010101111001,
      17'b00001101001111010,
      17'b00101010001011011,
      17'b00101101101011101,
      17'b00101110101011100,
      17'b00100110000111011,
      17'b00101001000100111,
      17'b00101101000001010,
      17'b00100000100101100,
      17'b00100100100101110,
      17'b00100101000101111,
      17'b00000110100001001,
      17'b00010100100100000,
      17'b00110011000001000,
      17'b10000111001111110,
      17'b10001011000100001,
      17'b10001111001000000,
      17'b10010011000100011,
      17'b10010010100100100,
      17'b10010111000100101,
      17'b10011011001011110,
      17'b10011110100100110,
      17'b10011111000101010,
      17'b10100011000101000,
      17'b10100010100101001,
      17'b10100111001011111,
      17'b10101010100101011,
      17'b10001110001000001,
      17'b10011001001000010,
      17'b10010000101000011,
      17'b10010001101000100,
      17'b10010010001000101,
      17'b10010101101000110,
      17'b10011010001000111,
      17'b10011001101001000,
      17'b10100001101001001,
      17'b10011101101001010,
      17'b10100001001001011,
      17'b10100101101001100,
      17'b10011101001001101,
      17'b10011000101001110,
      17'b10100010001001111,
      17'b10100110101010000,
      17'b10001010101010001,
      17'b10010110101010010,
      17'b10001101101010011,
      17'b10010110001010100,
      17'b10011110001010101,
      17'b10010101001010110,
      17'b10001110101010111,
      17'b10010001001011000,
      17'b10011010101011001,
      17'b10001101001011010,
      17'b10101010001111011,
      17'b10101101101111101,
      17'b10101110101111100,
      17'b10100110000111010,
      17'b10101001000100010,
      17'b10101101000001010,
      17'b10100000100111100,
      17'b10100100100111110,
      17'b10100101000111111,
      17'b10000110100001001,
      17'b10010100100100000,
      17'b10110011000001000
    })
  );

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

  // key_norm_data changing means key_norm_s 1 -> 0 -> 1
  // so we can impl a counter outside the ps2_system
  reg [7:0] key_norm_cnt_reg = 0;
  reg norm_hist_reg = 0;
  always @(posedge clk) begin
    if (~rstn) begin
      key_norm_cnt_reg <= 0;
      norm_hist_reg <= 0;
    end else begin
      if (norm_hist_reg == 0 & key_norm_s == 1) begin
        key_norm_cnt_reg <= key_norm_cnt_reg + 1;
      end
      norm_hist_reg <= key_norm_s;
    end
  end

  wire [7:0] key_norm_cnt = key_norm_cnt_reg;
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
