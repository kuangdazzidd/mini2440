.text
.global _start
_start:
	ldr	r0, =0x53000000
	mov r1, #0x00
	str	r1, [r0]		@�رտ��Ź�
	
	ldr	sp, =0x40001000	@���ö�ջ
	bl	main
	
halt_loop:
	b	halt_loop