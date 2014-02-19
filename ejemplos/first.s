;
; Simple pipeline test
;
        .data
fred:   .word 0
        .text

mike:   daddi r2,r0,2
        daddi r3,r0,10
        dsrl  r3,r3,1
        dadd r1,r2,r3
        dadd r1,r1,r3
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r3,r3
        lw   r1,fred(r0)
        beqz r1,mike
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r3,r3

        halt

