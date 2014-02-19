;
; Insertion sort algorithm
; See http://www.cs.ubc.ca/spider/harrison/Java/InsertionSortAlgorithm.java.html
;
	.data
array:  .word 0x11,0x111,0x1111111,0x11111,0x1,0x111111111,0x111111,0x111111111,0x111,0x11,0x1111,0x11111111,0x111,0x111111,0x111,0x11111111,0x111111111,0,0x1,0x1111111
len:    .word 20

        .text
        daddi r1,r0,8    ; r1 = i = 8
        ld r2,len(r0)    ; r2 = len
        dsll r2,r2,3     ; r2=len*8
for:    slt r3,r1,r2	 ; i < len?
        beqz r3,out	 ; yes - exit
        dadd r6,r0,r1    ; r6=j=i
        ld r4,array(r1)  ; r4=B=a[i]
loop:   slt r3,r0,r6	 ; j>0 ?
        beqz r3,over	 ; no -exit
        daddi r7,r6,-8	 ; r7=j-1
        ld r5,array(r7)	 ; get r5=a[j-1]
        slt r3,r4,r5     ; >B ?
        beqz r3,over	 
        sd r5,array(r6)	 ; a[j]=a[j-1]
        dadd r6,r0,r7	 ; j--
        j loop

over:   sd r4,array(r6)  ; a[j] = B
	daddi r1,r1,8    ; i++
        j for
out:    halt
   
