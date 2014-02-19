;int gcd(int a,int b)
;{
; while (a !=b)do
; {
;  if (a >b)
;  a =a -b;
;  else
;  b =b -a;
; }
; return a;
;}

        .data 
    u:  .word 0xdeadbeefdeadbee
    v:  .word 0xdead1B57C17008
        .text

	ld r1,u(r0)
	ld r2,v(r0)

loop:	beq r1,r2,out

	slt r3,r1,r2
	bnez r3,over
	dsub r1,r1,r2
	j loop
over:	dsub r2,r2,r1
	j loop

out:    halt