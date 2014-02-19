	.data 

texto:	 .asciiz  "Hola, Mundo!"    ; El mensaje a mostrar 
numero:  .word   125
flotante: .double 3.14159

CONTROL: .word32  0x10000 
DATA:	 .word32  0x10008 
   
	.text 

  lwu	$s0, DATA(r0)  ; $s0 = dirección de DATA 
  daddi	$t0, $0, texto  ; $t0 = dirección del mensaje a mostrar 
  sd	$t0, 0($s0)  ; DATA recibe el puntero al comienzo del mensaje 
 
  lwu	$s1, CONTROL(r0)  ; $s1 = dirección de CONTROL 
  daddi	$t0, $0, 6  ; $t0 = 6 -> función 6: limpiar pantalla alfanumérica 
  sd	$t0, 0($s1)  ; CONTROL recibe 6 y limpia la pantalla 
 
  daddi	$t0, $0, 4  ; $t0 = 4 -> función 4: salida de una cadena ASCII 
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida del mensaje 

  ld	$t0, numero($0)
  sd	$t0, 0($s0)  ; DATA recibe el numero 
  
  daddi	$t0, $0, 2  ; $t0 = 2 -> función 2: salida de entero sin signo
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida 

  l.d	f1, flotante($0)
  s.d	f1, 0($s0)  ; DATA recibe el numero en punto flotante
  
  daddi	$t0, $0, 3  ; $t0 = 3 -> función 4: salida de un punto flotante
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida

  halt 
