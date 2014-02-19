		.data
	x:	.word 0x12345678,0x12345678,0x12345678,0x12345678
	y:	.word 0x12345678,0x12345678,0x12345678,0x12345678
	r:	.word 0,0,0,0,0,0,0,0
		.code
		daddi r1,r0,0  ; i
		daddi r2,r0,0  ; j 
		daddi r10,r0,32 ; four	
		daddi r11,r0,1
		dsll r11,r11,16
		dsll r11,r11,16
		daddi r11,r11,-1

	loopx:	daddi r3,r0,0 ; carry
		ld r4,x(r1)
		daddi r2,r0,0     ; j=0
	loopy:	ld r5,y(r2)
		dmulu r6,r4,r5    ; x[i]*y[j]
		dadd r6,r6,r3	  ; x[i]*y[i]+carry
		dadd r7,r1,r2     ; i+j
		ld r8,r(r7)       ; get r[i+j]
		dadd r6,r6,r8     ; x[i]*y[i]+carry+r[i+j]
		and r8,r6,r11     ; d&0xffffffff
		sd r8,r(r7)
		dsrl r3,r6,16
		dsrl r3,r3,16
		daddi r2,r2,8
		bne r10,r2,loopy
		daddi r7,r1,32
		
		sd r3,r(r7)
		daddi r1,r1,8
		bne r10,r1,loopx
		halt

