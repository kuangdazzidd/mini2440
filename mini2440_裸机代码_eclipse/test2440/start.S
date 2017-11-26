.text

.global _start
_start:
    ldr     pc, _reset_addr
    ldr     pc, _undf_addr        /* 未定义  */
    ldr     pc, _swi_addr         /* 软中断 */
    ldr     pc, _pabt_addr        /* 取指中止 */
    ldr     pc, _dabt_addr        /* 取数据中止 */
    ldr     pc, _resv_addr        /* 取数据中止 */
    ldr     pc, _irq_addr         /* IRQ中断 */
    ldr     pc, _fiq_addr         /* 快中断 */

_reset_addr:    .word (init)
_undf_addr:     .word (undefined_instruction_handle)
_swi_addr:      .word (swi_handle)
_pabt_addr:     .word (prefetch_abort_handle)
_dabt_addr:     .word (data_abort_handle)
_resv_addr:     .word (undef_handle)
_irq_addr:      .word (irq_handle)
_fiq_addr:      .word (fiq_handle)

init:
    ldr    r0, =0x53000000
    mov    r1, #0x00
    str    r1, [r0]        @关闭看门狗

    msr cpsr_c, #0xd2
    ldr sp, = 0x33000000

    msr cpsr_c, #0x53
    ldr    sp, =0x34000000    @设置堆栈
    bl     main

halt_loop:
    b    halt_loop

undefined_instruction_handle:
    b    undefined_instruction_handle

swi_handle:
b    swi_handle

prefetch_abort_handle:
    b    prefetch_abort_handle

data_abort_handle:
b    data_abort_handle

undef_handle:
    b    undef_handle

fiq_handle:
b    fiq_handle

irq_handle:
    sub lr, lr, #4                  @ 计算返回地址
    stmdb   sp!,    { r0-r12,lr }   @ 保存使用到的寄存器
                                    @ 注意，此时的sp是中断模式的sp
                                    @ 初始值是上面设置的3072

    ldr lr, =int_return             @ 设置调用ISR即EINT_Handle函数后的返回地址
    @ldr pc, =EINT_Handle            @ 调用中断服务函数，在interrupt.c中
int_return:
    ldmia   sp!,    { r0-r12,pc }^  @ 中断返回, ^表示将spsr的值复制到cpsr








