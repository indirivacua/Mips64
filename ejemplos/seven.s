;
; Simple pipeline test
;
        .data
fred:   .word 0
        .text

mike:   mul.d  f0,f2,f4
        add.d  f2,f4,f6
        l.d    f4,fred(r0)
        s.d    f8,fred(r0)
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r3,r3

        halt

