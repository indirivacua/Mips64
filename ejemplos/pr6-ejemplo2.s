	.data 

preg:	 .asciiz  "Ingrese un numero: "  
pfijo:	 .asciiz  "Como entero: " 
pflot:	 .asciiz  "Como punto flotante: " 

CONTROL: .word32  0x10000 
DATA:	 .word32  0x10008 
   
	.text 

  lwu	$s0, DATA(r0)  ; $s0 = dirección de DATA 
  lwu	$s1, CONTROL(r0)  ; $s1 = dirección de CONTROL 

  daddi	$t0, $0, 6  ; $t0 = 6 -> función 6: limpiar pantalla alfanumérica 
  sd	$t0, 0($s1)  ; CONTROL recibe 6 y limpia la pantalla 
 
  daddi	$t0, $0, preg  ; $t0 = dirección del mensaje a mostrar 
  sd	$t0, 0($s0)  ; DATA recibe el puntero al comienzo del mensaje 
 
  daddi	$t0, $0, 4  ; $t0 = 4 -> función 4: salida de una cadena ASCII 
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida del mensaje 

  daddi	$t0, $0, 8  ; $t0 = 2 -> función 8: entrada de entero
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida 

  ld	$s2, 0($s0) 
  l.d	f1, 0($s0) 

  daddi	$t0, $0, pfijo  ; $t0 = dirección del mensaje a mostrar 
  sd	$t0, 0($s0)  ; DATA recibe el puntero al comienzo del mensaje 
  daddi	$t0, $0, 4  ; $t0 = 4 -> función 4: salida de una cadena ASCII 
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida del mensaje 

  sd    $s2, 0($s0)
  daddi	$t0, $0, 2  ; $t0 = 2 -> función 2: salida de entero sin signo
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida 




  daddi	$t0, $0, pflot  ; $t0 = dirección del mensaje a mostrar 
  sd	$t0, 0($s0)  ; DATA recibe el puntero al comienzo del mensaje 
  daddi	$t0, $0, 4  ; $t0 = 4 -> función 4: salida de una cadena ASCII 
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida del mensaje 

  s.d    f1, 0($s0)
  daddi	$t0, $0, 3  ; $t0 = 3 -> función 4: salida de un punto flotante
  sd	$t0, 0($s1)  ; CONTROL recibe 4 y produce la salida 
  halt 
