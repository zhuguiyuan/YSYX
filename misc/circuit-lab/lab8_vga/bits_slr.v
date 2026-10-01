`timescale 1ns / 1ps

module bits_slr #(
    parameter WIDTH = 1,
    parameter DEPTH = 1
) (
    input  wire             clk_i,
    input  wire [WIDTH-1:0] d_i,
    output wire [WIDTH-1:0] q_o
);

    generate
        if (DEPTH == 0) begin : g_bypass
            wire _unused = clk_i;
            assign q_o = d_i;
        end else begin : g_slr
            (* shreg_extract = "yes" *)
            reg [WIDTH-1:0] sr[DEPTH];

            integer i;
            always @(posedge clk_i) begin
                sr[0] <= d_i;
                for (i = 1; i < DEPTH; i++) begin
                    sr[i] <= sr[i-1];
                end
            end

            assign q_o = sr[DEPTH-1];
        end
    endgenerate

endmodule
