.data
value: .double 5.8
store: .double 3.9

.text
        l.d f5,value(r0)
        l.d f4,store(r0)
        jal  fred
        halt

fred:   div.d f6,f5,f4
        add.d f6,f5,f4
        mul.d f6,f5,f4
        jr  r31
        halt


