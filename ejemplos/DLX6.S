;
; test program
;

        .data
fred:   .space 5
        .space 3
joe:    .asciiz "joeyo"
        .ascii "hi"
        .ascii "joe"
        .byte 2
        .align 8
        .asciiz " Michael"
me:     .word32 joe
        .byte 3
nux:    .word 32
        .text
        daddi    r4,r0,0
        daddi    r5,r0,#7
        daddi    r3,r0,-1
        daddui   r4,r0,0xffff
        lui     r5,0xCCCC
        lw      r1,nux(r0)
        nop
lop:    ; old shit
loop2:  daddi    r2,r0,#99
loop:   lw      r1,-23(r0)
        dadd     r4,r4,r1
        bnez    r2,loop
        daddi    r2,r2,#-1     ; a comment!

        dadd     r5,r5,r4
        beqz    r5,lop
        j       loop3
        daddi    r3,r3,-1
        sw      r5,joe(r0)
loop3:  halt


