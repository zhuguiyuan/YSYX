.global _start

_start:
    lui a0, 0x2333
    addi a1, zero, -20
    jalr %lo(fun)(zero)
    lw a1, 4(zero)
    lbu a2, 0(zero)
    addi a2, a2, 1
    sb a2, 0(zero)
    addi a2, a2, -1
    sw a2, 0(zero)
    lw a1, 0(zero)
    jalr %lo(halt)(zero)

halt:
    jalr zero, %lo(halt)(zero)

fun:
    add a0, a0, a1
    sw a0, 0(zero)
    sb a0, 4(zero)
    ret
