        .data
;z:      .double 1.0
        .text
;        l.d f4,z(r0)
        div.d f0,f2,f4
        add.d f10,f0,f8
        sub.d f12,f8,f14
        halt

