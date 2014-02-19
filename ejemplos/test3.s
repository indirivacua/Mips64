.data
value: .double 5.8
store: .double 3.9

.text
        mul.d f0,f4,f6
        dadd R1,R2,R3
        dadd r1,r2,r3
        add.d f2,f4,f6
        dadd r0,r0,r0
        dadd r0,r0,r0
        l.d f2,store(r0)
        halt


