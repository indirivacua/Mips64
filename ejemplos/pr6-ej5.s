		.data
clave:		.asciiz	"pass"
control:		.word32	0x10000
data:		.word32	0x10008
error:		.asciiz	"Error!"
ok:		.asciiz	"Bienvenido"

		.code
		lwu	$s6, data ($0)
		lwu	$s7, control ($0)
		jal	char
		halt

char:		daddi	$t6, $0, 0
		daddi	$t7, $0, 4
Lazo:		daddi	$t0, $0, 6
		sd	$t0, 0($s7)
		daddi	$t1, $0, 9
		sd	$t1, 0($s7)
		lb	$s1, 0($s6)
		lb	$s2, clave ($t6)
		bne	$s1, $s2, rut_Error
		daddi	$t7, $t7,-1
		daddi	$t6, $t6, 1
		bnez	$t7, loop
		daddi	$a0, $0, 1
		j	respuesta
rut_Error:	daddi	$a0, $0, 6
		j	respuesta
respuesta:	daddi	$t0, $0, 6
		sd	$t0, 0($s7)
		beqz	$a0, error
		daddi	$t7, $0, ok
		sd	$t7, 0($s6)
		daddi	$t0, $0, 4
		sd	$t0, 0($s7)
		j	fin
error:		daddi	$t0, $0, 6
		sd	$t0, 0($s7)
		daddi	$t7, $0, error
		sd	$t7, 0($s6)
		daddi	$t0, $0, 4
		sd	$t0, 0($s7)
fin:		jr	$ra
