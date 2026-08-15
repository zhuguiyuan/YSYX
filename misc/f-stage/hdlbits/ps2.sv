module top_module (
    input clk,
    input [7:0] in,
    input reset,
    output [23:0] out_bytes,
    output done
);

    enum {
        BYTE1,
        BYTE2,
        BYTE3
    }
        state_reg, state_nxt;

    always_ff @(posedge clk) begin
        state_reg <= state_nxt;
        if (reset) state_reg <= BYTE1;
    end

    always_comb begin
        case (state_reg)
            BYTE1: begin
                if (in[3]) state_nxt = BYTE2;
                else state_nxt = BYTE1;
            end
            BYTE2: state_nxt = BYTE3;
            BYTE3: state_nxt = BYTE1;
        endcase
    end

    logic done_reg;
    always_ff @(posedge clk) begin
        done_reg <= (state_reg == BYTE3);
        if (reset) done_reg <= 1'b0;
    end
    assign done = done_reg;

    logic [24-1:0] out_reg;
    always @(posedge clk) begin
        out_reg <= {out_reg[15:0], in};
    end
    assign out_bytes = out_reg;

endmodule
