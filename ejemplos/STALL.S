        .data
mike:   .word 4

        .text
        lw      r5,mike(r0)
        xor     r10,r5,r4
        lw      r4,mike(r0)
        bnez    r4,loop
        nop
        nop
loop:   halt

