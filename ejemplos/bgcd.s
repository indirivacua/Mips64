;
; Nassir Hussain
; Binary GCD algorithm. Knuth Vol. 2
;
        .data 
;    u:  .word 0xdeadbeefdeadbee
;    v:  .word 0xdead1B57c17008
;u:	.word 0x30
;v:	.word 0x20
u:	.word 40902
v:	.word 24140

	.text

        ld r2,u(r0)
        ld r3,v(r0)

B1:     andi r6,r2,1
    ;    andi r7,r3,1
        bnez r6,B2           ; note jumps to different destinations
	andi r7,r3,1
        bnez r7,B2a
	dadd r0,r0,r0
        dsrl r2,r2,1
        dsrl r3,r3,1
        j B1
	daddi r5,r5,1
B2:     j B4
 	dsub r4,r0,r3
B2a:    dadd r4,r0,r2

B3:     dsra r4,r4,1         ; r4=t
B4:     andi r9,r4,1
        slt r10,r4,r0        ; useful way to fill a stall !
        beqz r9,B3
	dadd r0,r0,r0

B5:     beqz r10,here
	dadd r0,r0,r0
        j B6
	dsub r3,r0,r4
here:   dadd r2,r4,r0
B6:     dsub r4,r2,r3
        bnez r4,B3
	dadd r0,r0,r0
        dsllv r10,r2,r5        ; r=u << r5

        halt

