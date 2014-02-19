;
; double loop
;
        .data
number: .double 0.1,0.2,2.3,4.5,6.7,8.9          ; this is input number - change it!
last:   .double 29.54
scalar: .double 0.876
        .word 1,2,3,4  
        .text

        ld  r1,last(r0)
        ld  r2,number(r0)
        daddui R2,R2,-8 
        l.d f2,scalar(r0)
loop:   l.d   F0,0(R1)
        add.d F4,F0,F2
        s.d   F4,0(R1)
        daddui R1,R1,-8
        bne r1,r2,loop
        halt

