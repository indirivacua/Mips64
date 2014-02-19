;
; Hailstone numbers iteration
; If number is odd, multiply by 3 and add 1
; If number is even, divide it by 2
; repeat this iteration until number is 1 
;
        .data
number: .word 27          ; this is input number - change it!
max:    .word 0           ; max number so far
numb:   .double 0.0
        .text
start:  ld r1,number(r0)  ; program start
loop:   andi r3,r1,1      ; test odd or even
        beq r3,r0,even
odd:    dadd r2,r1,r1      ; times 2
        dadd r1,r2,r1      ; times 3
        daddi r1,r1,1      ; plus 1
        j over
even:   
        dsrl r1,r1,1      ; divide by 2
over:   ld r4,max(r0)     

        slt r3,r4,r1      ; compare with max
        beq r3,r0,skip
        sd r1,max(r0)     ; new max
skip:   slti r3,r1,2      ; test for finished
        beq r3,r0,loop
        halt

