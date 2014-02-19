;
; Simple pipeline test
;
        .data
fred:   .word 0
        .text

mike:   daddi r2,r0,2
        dadd r1,r2,r3
        dadd r1,r1,r3
        jal sub
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r3,r3
        halt

sub:    dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r2,r3
        dadd r1,r3,r3
        jr r31


