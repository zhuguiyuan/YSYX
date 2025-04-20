module alu (
    input  wire [2:0] op,
    input  wire [3:0] a,
    input  wire [3:0] b,
    output wire [3:0] r,
    output wire       cf,
    output wire       of,
    output wire       zf
);

    // pre-compute add and sub
    wire [3:0] not_b = ~b;
    wire [3:0] r_add, r_sub;
    wire       cf_add, cf_sub;

    assign {cf_add, r_add} = a + b;
    assign {cf_sub, r_sub} = a + not_b + 1;

    // for always block
    reg [3:0] r_inner;
    reg       cf_inner, of_inner, zf_inner;

    always @(*) begin
        {cf_inner, r_inner} = 0;
        case (op)
            3'b000: {cf_inner, r_inner} = {cf_add, r_add};
            3'b001: {cf_inner, r_inner} = {cf_sub, r_sub};
            3'b010: r_inner = ~a;
            3'b011: r_inner = a & b;
            3'b100: r_inner = a | b;
            3'b101: r_inner = a ^ b;
            3'b110: r_inner = {3'b0, $signed(a) < $signed(b)};
            3'b111: r_inner = {3'b0, r_sub == 0};
        endcase
    end

    // bypass zf_inner to get better performance
    always @(*) begin
        case (op)
            3'b000: zf_inner = r_add == 0;
            3'b001: zf_inner = r_sub == 0;
            default zf_inner = r_inner == 0;
        endcase
    end

    always @(*) begin
        case (op)
            3'b000: of_inner = a[3] == b[3] && a[3] != r_add[3];
            3'b001: of_inner = a[3] != b[3] && a[3] != r_sub[3];
            default: of_inner = 0;
        endcase
    end

    assign r = r_inner;
    assign cf = cf_inner;
    assign of = of_inner;
    assign zf = zf_inner;

endmodule
