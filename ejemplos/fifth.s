;
; Simple pipeline test
;
        .data
fred:   .word 0
        .text

        l.d f4,fred(r0)
        mul.d f0,f4,f6
        add.d f2,f0,f8
        s.d f2,fred(r0)
        dadd r1,r2,r3
        dadd r4,r5,r6
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0

        halt

