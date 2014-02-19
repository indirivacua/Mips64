;
;  Shaker Sort algorithm
;  see  http://www.cs.ubc.ca/spider/harrison/Java/ShakerSortAlgorithm.java.html
;
	.data
array:  .word 0x11,0x111,0x1111111,0x11111,0x1,0x111111111,0x111111,0x111111111
        .word 0x111,0x11,0x1111
        .word 0x11111111,0x111,0x111111,0x111,0x11111111,0x111111111,0,0x1
	.word 0x1111111
len:	.word 20

        .text
        dadd r1,r0,r0    ; r1 = i = 0
        ld r2,len(r0)    ; r2 = k
        dsll r2,r2,3     ; r2=k*8

loop:   slt r3,r1,r2
        beqz r3,out
        dadd r4,r0,r1    ; r4 = mn
        dadd r5,r0,r1    ; r5 = mx
        dadd r6,r0,r1    ; r6 = j = i
for:    slt r3,r6,r2
        beqz r3,over
        ld r7,array(r6)  ; array[j]
        ld r8,array(r4)  ; array[mn]
        ld r9,array(r5)  ; array[mx]
        slt r3,r7,r8
        movn r4,r6,r3
        slt r3,r9,r7
        movn r5,r6,r3
        daddi r6,r6,8    ; j++
        j for
		
over:   ld r8,array(r4)  ; array[min]
        ld r10,array(r1) ; array[i]
        sd r10,array(r4)
        sd r8,array(r1)

        daddi r2,r2,-8

        bne r5,r1,else
        ld r8,array(r4)
        ld r10,array(r2)
        sd r10,array(r4)
        sd r8,array(r2)
        daddi r1,r1,8
        j loop
else:   ld r9,array(r5)
        ld r10,array(r2)
        sd r10,array(r5)
        sd r9,array(r2)
        daddi r1,r1,8
        j loop

out:    halt