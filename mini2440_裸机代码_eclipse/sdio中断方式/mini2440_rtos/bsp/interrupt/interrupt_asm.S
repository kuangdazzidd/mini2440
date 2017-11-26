

#define NOINT           0xc0

@ c语言调用函数r0作为第一个参数
@ r1作为返回值

/*
  C 控制域屏蔽字节(psr[7:0])
  X 扩展域屏蔽字节(psr[15:8])
  S 状态域屏蔽字节(psr[23:16])
  F 标志域屏蔽字节(psr[31:24])
*/

/*
 * disable interrupt
 */
.globl s3c2440_interrupt_disable
s3c2440_interrupt_disable:
    mrs r0, cpsr            @ 读cpsr寄存器到r0
    orr r1, r0, #NOINT      @ 逻辑与操作  r0 | 0xc0  禁止中断
    msr cpsr_c, r1          @ 写r1到状态寄存器
    mov pc, lr

/*
 * enable interrupt
 */
.globl s3c2440_interrupt_enable
s3c2440_interrupt_enable:
    msr cpsr, r0        @ 中断位写1清零
    mov pc, lr
