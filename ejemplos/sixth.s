;
; Simple pipeline test
;
        .data
fred:   .word 0
        .text

mike:   daddi r2,r0,2
        daddi r3,r0,3  
        dadd r1,r2,r3
        ld r4,0(r1)
        sd r4,12(r1)
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r3,r3

        halt

