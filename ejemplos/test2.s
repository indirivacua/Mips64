.data
value: .double 5.8
store: .double 3.9

.text
        l.d f4,store(r0)
        mul.d f0,f4,f6
        add.d f2,f0,f8
        s.d f2,value(r0)
        halt


