.text
.global _start
_start:
	ldr	r0, =0x53000000
	mov r1, #0x00
	str	r1, [r0]		@关闭看门狗
	
	ldr	sp, =0x40001000	@设置堆栈
	bl	main
	
halt_loop:
	b	halt_loop