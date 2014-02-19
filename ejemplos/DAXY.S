        .data
x:      .double 1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9
y:      .double 5,6,7,8,9,10,11,12,13,14
a:      .double 2
        .text
        dadd r1,r0,r0 ; r1 = i
        daddi r2,r0,80
        l.d f0,a(R0)
foo:    l.d f2,x(r1)
        mul.d f4,f2,f0
        l.d f6,y(r1)
        add.d f6,f4,f6
        s.d f6,y(r1)
        daddui r1,r1,8
        bne r1,r2,foo
        halt



