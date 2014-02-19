;
; Simple pipeline test
;
        .data
fred:   .word 3
        .text

back:   daddi r2,r0,2
        daddi r3,r0,3
        dadd r1,r2,r3
        beq r2,r3,back
        lui  r7,6
        dsub r1,r1,r3
        lui  r8,11
        halt

