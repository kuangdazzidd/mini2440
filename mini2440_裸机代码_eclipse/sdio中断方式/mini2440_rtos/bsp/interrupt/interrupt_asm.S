

#define NOINT           0xc0

@ c���Ե��ú���r0��Ϊ��һ������
@ r1��Ϊ����ֵ

/*
  C �����������ֽ�(psr[7:0])
  X ��չ�������ֽ�(psr[15:8])
  S ״̬�������ֽ�(psr[23:16])
  F ��־�������ֽ�(psr[31:24])
*/

/*
 * disable interrupt
 */
.globl s3c2440_interrupt_disable
s3c2440_interrupt_disable:
    mrs r0, cpsr            @ ��cpsr�Ĵ�����r0
    orr r1, r0, #NOINT      @ �߼������  r0 | 0xc0  ��ֹ�ж�
    msr cpsr_c, r1          @ дr1��״̬�Ĵ���
    mov pc, lr

/*
 * enable interrupt
 */
.globl s3c2440_interrupt_enable
s3c2440_interrupt_enable:
    msr cpsr, r0        @ �ж�λд1����
    mov pc, lr
