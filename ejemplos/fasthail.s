;
; Hailstone numbers iteration
; If number is odd, multiply by 3 and add 1
; If number is even, divide it by 2
; repeat this iteration until number is 1 
;
; This version has been re-organised to minimize pipeline stalls
; Gets rid of almost all load-stalls
;
        .data
number: .word 27          ; this is input number - change it!
max:    .word 0           ; max number so far
        .text
  
start:  ld r1,number(r0)  ; program start
        andi r5,r1,1      ; test odd or even
loop:   dadd r2,r1,r1     ; times 2
        beqz r5,even
odd:    dadd r1,r2,r1     ; times 3
        ld r4,max(r0)     
        daddi r1,r1,1     ; plus 1
        slt r3,r4,r1      ; compare with max
        andi r5,r1,1
        beqz r3,loop
        sd r1,max(r0)     ; new max
        j loop
even:   dsrl r1,r1,1      ; divide by 2
        slti r3,r1,2      ; test for finished
        andi r5,r1,1
        beqz r3,loop
        halt
 


