// 选择器模板内部实现
module MuxKeyInternal #(
    NR_KEY      = 2,
    KEY_LEN     = 1,
    DATA_LEN    = 1,
    HAS_DEFAULT = 0
) (
    output reg [                   DATA_LEN-1:0] value_o,
    input      [                    KEY_LEN-1:0] key_i,
    input      [                   DATA_LEN-1:0] default_i,
    input      [NR_KEY*(KEY_LEN + DATA_LEN)-1:0] lut_i
);

    localparam PAIR_LEN = KEY_LEN + DATA_LEN;
    wire [PAIR_LEN-1:0] pair_list[NR_KEY-1:0];
    wire [ KEY_LEN-1:0] key_list [NR_KEY-1:0];
    wire [DATA_LEN-1:0] data_list[NR_KEY-1:0];

    genvar n;
    generate
        for (n = 0; n < NR_KEY; n = n + 1) begin
            assign pair_list[n] = lut_i[PAIR_LEN*(n+1)-1 : PAIR_LEN*n];
            assign data_list[n] = pair_list[n][DATA_LEN-1:0];
            assign key_list[n]  = pair_list[n][PAIR_LEN-1:DATA_LEN];
        end
    endgenerate

    reg [DATA_LEN-1 : 0] lut_out;
    reg hit;
    integer i;
    always @(*) begin
        lut_out = 0;
        hit = 0;
        for (i = 0; i < NR_KEY; i = i + 1) begin
            lut_out = lut_out | ({DATA_LEN{key_i == key_list[i]}} & data_list[i]);
            hit = hit | (key_i == key_list[i]);
        end
        if (!HAS_DEFAULT) value_o = lut_out;
        else value_o = (hit ? lut_out : default_i);
    end
endmodule

// 不带默认值的选择器模板
module MuxKey #(
    NR_KEY   = 2,
    KEY_LEN  = 1,
    DATA_LEN = 1
) (
    output [                   DATA_LEN-1:0] value_o,
    input  [                    KEY_LEN-1:0] key_i,
    input  [NR_KEY*(KEY_LEN + DATA_LEN)-1:0] lut_i
);
    MuxKeyInternal #(NR_KEY, KEY_LEN, DATA_LEN, 0) i0 (
        value_o,
        key_i,
        {DATA_LEN{1'b0}},
        lut_i
    );
endmodule

// 带默认值的选择器模板
module MuxKeyWithDefault #(
    NR_KEY   = 2,
    KEY_LEN  = 1,
    DATA_LEN = 1
) (
    output [                   DATA_LEN-1:0] value_o,
    input  [                    KEY_LEN-1:0] key_i,
    input  [                   DATA_LEN-1:0] default_i,
    input  [NR_KEY*(KEY_LEN + DATA_LEN)-1:0] lut_i
);
    MuxKeyInternal #(NR_KEY, KEY_LEN, DATA_LEN, 1) i0 (
        value_o,
        key_i,
        default_i,
        lut_i
    );
endmodule
