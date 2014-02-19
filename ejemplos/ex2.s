		.data
	n:	.word 8
	x:	.double 0.5 

		.text
		LD 		R1,n(R0)
		L.D 		F0,x(R0)
		DADDI 	R2, R0, 1	; R2 = 1
		MTC1 	F11, R2		; F11 = 1
		CVT.L.D 	F2,F11		; F2 = 1
	loop: 	MUL.D 	F2, F2, F0	; F2 = F2*F0
		DADDI  	R1, R1, -1	; decrement R1 by 1
		BNEZ  	R1, loop		; if R1 != 0 continue

; result in F2

		HALT

