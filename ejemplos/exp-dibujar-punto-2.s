	.data 
coorX:	.byte   24  ; coordenada X de un punto 
coorY:	.byte   24  ; coordenada Y de un punto 
color:	.byte   255, 255, 0, 0  ; color: máximo rojo, máximo verde, nada azul => amarillo

CONTROL: .word32 0x10000 
DATA:	 .word32 0x10008 
   
       .text 

       daddi $sp, $0, 0x400    ; Inicializa el puntero al tope de la pila
	   
       jal limpiar
	   
	   daddi $s5, $0, 5
	   
       lbu $s0, coorX($0)       
	   lbu $s1, coorY($0)
	   lwu $s2, color($0)
	   
	   
sigue: dadd $a0, $0, $s0
	   dadd $a1, $0, $s1
	   dadd $a2, $0, $s2
	   jal punto
	   
	   daddi $s0, $s0, 1
	   daddi $s1, $s1, 1
	   
	   daddi $s5, $s5, -1
	   bnez $s5, sigue	  
	   
       halt 


limpiar: daddi $sp, $sp, -8
	     sd $s0, 0($sp)
         lwu  $s0, CONTROL(r0)  ; $s0 = CONTROL 

         daddi  $t0, $0, 7  ; $t0 = 7 -> función 7: borrar pantalla
         sd  $t0, 0($s0)  ; CONTROL recibe 7 y borra la pantalla
	   
 	     ld $s0, 0($sp)
         daddi $sp, $sp, +8
         jr $ra


punto: daddi $sp, $sp, -16
	   sd $s0, 0($sp)
	   sd $s1, 8($sp)
       lwu  $s0, CONTROL($0)  ; $s0 = CONTROL 
       lwu  $s1, DATA($0)     ; $s1 = DATA 

       sb  $a0, 5($s1)  ; DATA+5 recibe el valor de coordenada X
	                    ; en $a0
       sb  $a1, 4($s1)  ; DATA+5 recibe el valor de coordenada Y
	                    ; en $a1
	   sw  $a2, 0($s1)  ; DATA+0 recibe el color, en $a2

       daddi  $t0, $0, 5  ; $t0 = 5 -> función 5: salida gráfica 
       sd  $t0, 0($s0)  ; CONTROL recibe 5 y produce el dibujo del punto 
	   
	   ld $s0, 0($sp)
	   ld $s1, 8($sp)     
       daddi $sp, $sp, +16
       jr $ra
	   