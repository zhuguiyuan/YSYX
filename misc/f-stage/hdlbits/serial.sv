module top_module (
    input clk,
    input in,
    input reset,  // Synchronous reset
    output [7:0] out_byte,
    output done
);

    wire odd;
    parity p_inst (
        .clk  (clk),
        .reset(reset || valid_bits[0] == 1'b0),
        .in   (in),
        .odd  (odd)
    );

    // error|end|parity|bit7|...|bit1|bit0|start
    logic [11-1:0] valid_bits;
    always_ff @(posedge clk) begin
        if (reset) valid_bits <= 11'b0;
        else if (valid_bits[0] == 1'b0 && in == 1'b0) begin
            valid_bits <= 11'b1;
        end else if (valid_bits[9] == 1'b1 && in == 1'b1) begin
            valid_bits <= 11'b0;
        end else if (valid_bits[0] == 1'b1) begin
            valid_bits <= {valid_bits[10-1:0], 1'b1};
        end
    end

    logic done_reg;
    always_ff @(posedge clk) begin
        if (reset) begin
            done_reg <= 1'b0;
        end else if (valid_bits[9] == 1'b1 && in == 1'b1 && valid_bits[10] == 1'b0 && odd) begin
            done_reg <= 1'b1;
        end else begin
            done_reg <= 1'b0;
        end
    end

    assign done = done_reg;

    logic [10-1:0] data_reg;
    always_ff @(posedge clk) begin
        if (valid_bits[0]) data_reg <= {in, data_reg[10-1:1]};
    end
    assign out_byte = data_reg[8-1:0];

endmodule
