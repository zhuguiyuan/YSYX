module alu (
    input  wire [2:0] op,
    input  wire [3:0] a,
    input  wire [3:0] b,
    output wire [3:0] r,
    output wire       cf,
    output wire       of,
    output wire       zf
);

    //         sub          || compare      || equal
    wire cin = op == 3'b001 || op == 3'b110 || op == 3'b111;
    wire [3:0] b_inner, alu_out;
    reg  [3:0] r_inner;

    assign b_inner = {4{cin}} ^ b;
    assign {cf, alu_out} = a + b_inner + {3'b0, cin};

    always @(*) begin
        case (op)
            3'b000: r_inner = alu_out;
            3'b001: r_inner = alu_out;
            3'b010: r_inner = ~a;
            3'b011: r_inner = a & b;
            3'b100: r_inner = a | b;
            3'b101: r_inner = a ^ b;
            3'b110: r_inner = {3'b0, $signed(a) < $signed(b)};
            3'b111: r_inner = {3'b0, alu_out == 0};
            default r_inner = 0;
        endcase
    end

    assign r = r_inner;
    assign of = (a[3] == b_inner[3]) && (a[3] != alu_out[3]);
    assign zf = r == 0;

endmodule
