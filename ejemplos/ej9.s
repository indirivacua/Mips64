.data
        CONTROL:      .word32 0x10000
        DATA:         .word32 0x10008
        color_punto:  .word32 0x804020
        colores:      .word32 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0x000000
        imagen:       .space 2500

.text
        daddi  $s0, $0, 0           ; Coordenada X del puntero.
        daddi  $s1, $0, 0           ; Coordenada Y del puntero.
        daddi  $s2, $0, 3           ; Índice del color actual.
        dadd   $s3, $0, $0          ; Dibujando? Inicialmente, no.
        daddi  $s4, $0, 50

        lwu    $s5, color_punto($0)
        lwu    $s6, CONTROL($0)
        lwu    $s7, DATA($0)

        daddi  $t1, $0, 97          ; Código ASCII de la 'a' -> izquierda
        daddi  $t2, $0, 115         ; Código ASCII de la 's' -> abajo
        daddi  $t3, $0, 100         ; Código ASCII de la 'd' -> derecha
        daddi  $t4, $0, 119         ; Código ASCII de la 'w' -> arriba
        daddi  $t7, $0, 32          ; Código ASCII de ' ' -> pintar/no pintar

        daddi  $t6, $0, 49          ; Límite de la pantalla

        daddi  $t5, $0, 5           ; Códigos de CONTROL
        daddi  $t9, $0, 9

loop:   dmul   $a0, $s1, $s4

        sw     $s5, 0($s7)          ; Dibuja el cursor en la posición actual.
        sb     $s1, 4($s7)
        sb     $s0, 5($s7)
        sd     $t5, 0($s6)

        sd     $t9, 0($s6)          ; Lee una tecla.
        lbu    $t0, 0($s7)

        dadd   $a0, $a0, $s0        ; Lee el color de la imagen sobre la que
        lbu    $a1, imagen($a0)     ; el cursor estaba parado.

        movn   $a1, $s2, $s3        ; Si está dibujando, pinta con el color actual.
        dsll   $a2, $a1, 2          ; Accede al color RGB a partir del índice almacenado.
        lwu    $a3, colores($a2)

        sw     $a3, 0($s7)          ; Pinta el pixel en pantalla
        sd     $t5, 0($s6)

        sb     $a1, imagen($a0)     ; Almacena el índice del color en la imagen

        beq    $t0, $t1, mover_izquierda
        beq    $t0, $t2, mover_abajo
        beq    $t0, $t3, mover_derecha
        beq    $t0, $t4, mover_arriba
        beq    $t0, $t7, pintar_no_pintar

        slti   $v0, $t0, 49         ; Compara la tecla con el '1'.
        slti   $v1, $t0, 57         ; Compara la tecla con el '9'.

        daddi  $a1, $t0, -49

        bnez   $v0, loop
        movn   $s2, $a1, $v1

        j      loop 

mover_izquierda:
        slt    $t8, $r0, $s0
        dsub   $s0, $s0, $t8
        j      loop

mover_derecha:
        slt    $t8, $s0, $t6
        dadd   $s0, $s0, $t8
        j      loop

mover_abajo:
        slt    $t8, $r0, $s1
        dsub   $s1, $s1, $t8
        j      loop

mover_arriba:
        slt    $t8, $s1, $t6
        dadd   $s1, $s1, $t8
        j      loop

pintar_no_pintar:
        xori   $s3, $s3, 1
        j      loop
