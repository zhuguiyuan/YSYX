module rom7seg (
    input  wire [4-1:0] data_i,
    output wire [8-1:0] data_o
);

    reg [8-1:0] code_imm;
    always @(*) begin
        case (data_i)
            4'h0: code_imm = ~8'b11111100;
            4'h1: code_imm = ~8'b01100000;
            4'h2: code_imm = ~8'b11011010;
            4'h3: code_imm = ~8'b11110010;
            4'h4: code_imm = ~8'b01100110;
            4'h5: code_imm = ~8'b10110110;
            4'h6: code_imm = ~8'b10111110;
            4'h7: code_imm = ~8'b11100000;
            4'h8: code_imm = ~8'b11111110;
            4'h9: code_imm = ~8'b11110110;
            4'hA: code_imm = ~8'b11101110;
            4'hB: code_imm = ~8'b00111110;
            4'hC: code_imm = ~8'b10011100;
            4'hD: code_imm = ~8'b01111010;
            4'hE: code_imm = ~8'b10011110;
            4'hF: code_imm = ~8'b10001110;
        endcase
    end

    assign data_o = code_imm;

endmodule
