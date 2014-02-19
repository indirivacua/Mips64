; DLX version of series.cpp

                .data
         x:     .double 0.6
         s:     .double 1.0

                .text
                L.D     F0,x(R0)     ;F0=x
                L.D     F4,s(R0)     ;F4=s
        
                DADDI   R1,R0,3

                MUL.D   F2,F0,F0  
                MTC1    F11,R1
                cvt.l.d F6,F11
                ADD.D   F4,F4,F0
                ADD.D   F4,F4,F0     ; 1+2*x

                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                DADDI   R1,R1,1
                MUL.D   F2,F0,F2
                ADD.D   F4,F4,F8

                MTC1    F11,R1
                CVT.L.D F6,F11
                MUL.D   F8,F6,F2
                ADD.D   F4,F4,F8

                HALT

