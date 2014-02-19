.data
value: .double 5.8
store: .double 0.0

.text
        l.d f0,value(r0)
        s.d f0,store(r0)
        mov.d f0, f5
        mul.d f6,f5,f4
        add.d f3,f2,f6
        div.d f9,f3,f2
        halt


