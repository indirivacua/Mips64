        .data
number: .word 10,20,30
fred:   .word 30,40,70
        .text

start:  daddi r29,r0,80   ; position stack
	dadd r1,r2,r3
	ld r1,number(r0)    ; get number
	ld r2,fred(r0)
        halt
