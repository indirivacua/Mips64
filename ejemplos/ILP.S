        .data
two:    .double 2.0
        .org 128
        .double 1.5
        .double 2.5
        .double 3.5
        .double 4.5

        .double 1.6
        .double 2.6
        .double 3.6
        .double 4.6

        .double 1.7
        .double 2.7
        .double 3.7
        .double 4.7

        .double 1.8
        .double 2.8
        .double 3.8
        .double 4.8

        .text
        DADDUI r1,r0,248
        DADDUI r2,r0,120
        L.D F2,two(r0)
loop:   L.D F0,0(R1)
        L.D F6,-8(R1)
        L.D F10,-16(R1)
        L.D F14,-24(R1)
        ADD.D F4,F0,F2
        ADD.D F8,F6,F2
        ADD.D F12,F10,F2
        ADD.D F16,F14,F2
        S.D F4,0(R1)
        S.D F8,-8(R1)
        S.D F12,-16(R1)
        S.D F16,-24(R1)
        DADDUI R1,R1,-32
        BNE R1,R2,loop
        HALT

