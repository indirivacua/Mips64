;Stephen Doyle
;54167434
; CA226 Assignment 1 2005/2006

.data

u:	.word 40902 ; number u
v:      .word 24140 ; number v

.text
		ld r4,u(r0) ; load value to register
		ld r5,v(r0) ; load value to register
		daddi r7,r0,0 ; p=0;
firstloop:
		andi r1,r4,1     	; test odd or even (on r4 (u))
		andi r2,r5,1     	; test odd or even (on r5 (v))
	        bnez r1,afterfirst     	; if r1 is not zero, r4 was odd (so break this loop)
	        bnez r2,afterfirst     	; if r2 is not zero, r5 was odd (so break this loop)
				
		dsra r4,r4,1    	 ; shift right divide ( /2)
		dsra r5,r5,1		 ; shift right diide ( /2)
		daddi r7,r7,1		 ; add 1 to p (this will be the power of 2 to multiply at the end)
		
		j firstloop 		; back to the loop
		
afterfirst:	andi r1,r4,1    	; AND with 1 to see if odd or even (if (u%2 ==0)
		dadd r6,r4,r0		; this line was after the jump, but to cut the stall its now here
					; if the branch isnt taken the dsub overwrites r6 with the correct value
	        beqz r1,secondloop   	; if r1 is zero, r4 was even, so t = u (r6:=r5) 
	        dsub r6,r0,r5		; r0 - r5 (0 - v) = r6 (t)
        	
secondloop:	beqz r6,aftersecond	

thirdloop:	andi r1,r6,1         	; AND with 1 to see if odd or even (if (t%2 ==0)
	        bnez r1,afterthird   	; if r1 is zero, r6 was even (so loop continue), if it wasnt, break
		dsra r6,r6,1         	; {else divide by 2, loop}
		j thirdloop	     	;                   


afterthird:	slt r2,r6,r0         	; if t<0, set r2 to 1
		beqz r2,tgreatzero   	; so if its 0, t is greater than 0
		dsub r5,r0,r6        	; v= -t
		j tlesszero

tgreatzero:	dadd r4,r6,r0	     	; u=t
tlesszero:	
		dsub r6,r4,r5	     	; t = u - v

		j secondloop
aftersecond:	dsllv r10,r4,r7	     	; shift left by p (p is a power of 2, from above)
		halt	 	     	; and we're done
		
		
		
		
;notes:

; shift left = multiply
; shift right = divide


;register conventions:
;r0 = 0
;r1 - 3 : loop conditions
;r4 = u
;r5 = v
;r6 = t
;r7  = p
;r10 = answer

;algorithm:
;    p=0;
;    for (k=0;;k++)
;    {
;        if (u%2==1 || v%2==1) break;
;        u/=2;
;        v/=2;
;        p+=2;
;    }
;    if (u%2==0) t=u;
;    else        t=-v;
;
;    while (t!=0)
;    {
;        while (t%2==0) t/=2;
;        if (t>0) u=t;
;        else     v=-t;
;        t=u-v;
;    }
;    r=u^p;		
;