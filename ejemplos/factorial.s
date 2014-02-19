;
; Factorial example
; returns number! in r10
;
	
	.data
number: .word 10
        .text

start:  daddi r29,r0,0x80   ; position a stack in data memory, use r29 as stack pointer
        ld r1,number(r0)    ; get number
        jal factorial
        halt


;
; Function to multiply the integers in r3 and r4
; returns product in r3
;

mul:    daddi r1,r0,64   ; r1=64 bits
        daddi r5,r0,63   ; for shifting
        daddu r2,r0,r0   ; r2=0
        daddu r8,r0,r0   ; r10=0
        andi r9,r3,1     ; check LSB of x
        dsrl r3,r3,1     ; and then shift it right
again:  daddu r6,r0,r0
        movn r6,r4,r9
        daddu r2,r2,r6
        sltu r7,r2,r6    ; did it overflow?
        dsllv r7,r7,r5   ; catch overflowed bit
        andi r8,r2,1     ; get LSB of r2 ..
        dsllv r8,r8,r5   ; .. becomes MSB of r3 
        dsrl r2,r2,1     ; 64-bit shift of r2,r3
        or r2,r2,r7      ; or in overflowed bit
        andi r9,r3,1     ; catch LSB
        daddi r1,r1,-1   ; here to avoid stall
        dsrl r3,r3,1
        or r3,r3,r8      ; shift it right, and set MSB
        bnez r1,again

        jr r31
        nop

;
; parameter passed in r1, return value in r10
;
factorial:  slti r10,r1,2
        bnez r10,out      ; set r10=1 and return if r1=1

        sd r31,(r29)
        daddi r29,r29,8   ; push return address onto stack

        sd r1,(r29)
        daddi r29,r29,8   ; push r1 on stack

        daddi r1,r1,-1    ; r1 = r1-1
        jal factorial     ; recurse...

        dadd r4,r0,r10
        daddi r29,r29,-8
        ld r3,(r29)       ; pop n off the stack

        jal mul           ; multiply r1 x factorial(r1-1)
        dadd r10,r0,r3    ; move product r3 to r10

        daddi r29,r29,-8  ; pop return address
        ld r31,0(r29)
out:    jr r31
	