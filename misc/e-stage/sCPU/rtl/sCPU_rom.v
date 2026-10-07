module sCPU_rom (
    input  wire [8-1:0] pc_i,
    output wire [8-1:0] inst_o
);

    reg [8-1:0] rom[0:15];
    initial begin
        integer i;

        rom[0]  = 8'b10000110;  // li r0, 8
        rom[1]  = 8'b10010010;  // li r1, 2
        rom[2]  = 8'b00000001;  // add r0, r0, r1
        rom[3]  = 8'b10010000;  // li r1, 0
        rom[4]  = 8'b10100000;  // li r2, 0
        rom[5]  = 8'b10110001;  // li r3, 1
        rom[6]  = 8'b00010111;  // add r1, r1, r3
        rom[7]  = 8'b00101001;  // add r2, r2, r1
        rom[8]  = 8'b11111001;  // bner0 r1, -2
        rom[9]  = 8'b01101000;  // io led
        rom[10] = 8'b01101001;  // io seg
        rom[11] = 8'b10000000;  // li r0, 0
        rom[12] = 8'b11000011;  // bner0 r3, 0

        for (i = 13; i < 16; i = i + 1) begin
            rom[i] = 8'd0;
        end
    end

    assign inst_o = rom[pc_i[4-1:0]];
    wire [4-1:0] unused_pc = pc_i[8-1:4];

endmodule
