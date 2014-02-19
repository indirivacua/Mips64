;
; Simple pipeline test
;
        .data
fred:   .word 0
        .text

        mul.d f0,f4,f6
        dadd r1,r2,r3
        dadd r4,r5,r6
        add.d f2,f4,f6
        dadd r1,r2,r3
        dadd r4,r5,r6
        l.d f2,fred(r0)
        dadd r1,r2,r3
        dadd r4,r5,r6
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0
        dadd r0,r0,r0

        halt

