.global _start

_start:
    lui x1, 0x12345
    addi x1, x1, 0x678
    sw x1, 0(zero)
    lw x2, 0(zero)
    lbu x4, 0(zero)
    lbu x5, 1(zero)
    lbu x6, 2(zero)
    lbu x7, 3(zero)
    li x2, 0x90
    sb x2, 3(zero)
    li x2, 0xab
    sb x2, 2(zero)
    li x2, 0xcd
    sb x2, 1(zero)
    li x2, 0xef
    sb x2, 0(zero)
    lw x1, 0(zero)
    jalr %lo(halt)(zero)

halt:
    jalr zero, %lo(halt)(zero)
