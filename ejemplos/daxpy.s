;
; Calculate Y=aX+Y
; Each element of y[i] is replaced with a*x[i]+y[i]
;
; This is the classic DAXPY loop as used in the Gaussian 
; elimination algorithm
;
; cycles=196
;

        .data
x:      .double  1, 2, 3, 4,  5,  6,  7,  8,  9, 10
y:      .double -2,-4,-6,-8,-10,-12,-14,-16,-18,-20
a:      .double 2
        .text
        dadd r1,r0,r0     ; r1 = i
        l.d f0,a(R0)      ; get a
foo:    l.d f2,x(r1)      ; get x[i]
        mul.d f4,f2,f0    ; calculate a.x[i]
        l.d f6,y(r1)      ; get y[i]
        add.d f6,f4,f6    ; a.x[i]+y[i]
        s.d f6,y(r1)      ; store y[i]
        daddi r1,r1,8     ; increment i
        slti r3,r1,80     ; check for finished the loop
        bnez r3,foo
        halt

