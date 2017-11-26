/*
 * mmu.c
 *
 *  Created on: 2017��7��6��
 *      Author: Administrator
 */


#include "mmu.h"


///* ���ƼĴ��� */
//#define GPBCON      (*(volatile unsigned long*)0x56000010)
///* ���ݼĴ��� */
//#define GPBDAT      (*(volatile unsigned long*)0x56000014)
///* �����Ĵ��� */
//#define GPBUP       (*(volatile unsigned long*)0x56000018)

#define __MMU_GPBCON    (*(volatile unsigned long*)0xA0000010)
#define __MMU_GPBDAT    (*(volatile unsigned long*)0xA0000014)
#define __MMU_GPBUP     (*(volatile unsigned long*)0xA0000018)


/*
 * ���ڶ���������һЩ�궨��
 */
#define MMU_FULL_ACCESS     (3 << 10)   /* ����Ȩ�� */
#define MMU_DOMAIN          (0 << 5)    /* �����ĸ��� */
#define MMU_SPECIAL         (1 << 4)    /* ������1 */
#define MMU_CACHEABLE       (1 << 3)    /* cacheable */
#define MMU_BUFFERABLE      (1 << 2)    /* bufferable */
#define MMU_SECTION         (2)         /* ��ʾ���Ƕ������� */
#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | \
                             MMU_SECTION)
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | \
                             MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
#define MMU_SECTION_SIZE    0x00100000  /* 1M */




int mmu_led_init()
{
    /* ��������LEDΪ��� */
    __MMU_GPBCON = 0x00015400;
    /* ��������LED���� */
    __MMU_GPBDAT = 0x000007ff;

    return 0;
}

int mmu_led_on(int led)
{
    __MMU_GPBDAT &= (~(0x01 << (led + 5)));
    return 0;
}


int mmu_led_off(int led)
{
    __MMU_GPBDAT |= 0x01 << (led + 5);
    return 0;
}




int mmu_init()
{
    unsigned long ttb = 0x30000000 + 0x00400000;
    unsigned long virtuladdr, physicaladdr;
    unsigned long *mmu_tlb_base = (unsigned long *)(ttb);   /* +4M*/

    /*
     * 0x56000000��GPIO�Ĵ�������ʼ�����ַ��
     * GPBCON��GPBDAT�������Ĵ����������ַ0x56000050��0x56000054��
     * Ϊ���ڵڶ����ֳ��������Ե�ַ0xA0000050��0xA0000054������GPFCON��GPFDAT��
     * �Ѵ�0xA0000000��ʼ��1M�����ַ�ռ�ӳ�䵽��0x56000000��ʼ��1M�����ַ�ռ�
     */
    virtuladdr = 0xA0000000;
    physicaladdr = 0x56000000;
    *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                            MMU_SECDESC;

    /*
     * SDRAM�������ַ��Χ��0x30000000��0x33FFFFFF��
     * �������ַ0x30000000��0x03FFFFFFӳ�䵽�����ַ0x30000000��0x33FFFFFF�ϣ�
     * �ܹ�64M���漰64����������
     */
    virtuladdr = 0x30000000;
    physicaladdr = 0x30000000;
    while (virtuladdr < 0x34000000)
    {
        *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                                MMU_SECDESC_WB;
        virtuladdr += 0x100000;
        physicaladdr += 0x100000;
    }

}


int mmu_start()
{
    unsigned long ttb = 0x30000000 + 0x00400000;

    __asm__ __volatile__ (
        "mov    r0, #0\n"
        "mcr    p15, 0, r0, c7, c7, 0\n"    /* ʹ��ЧICaches��DCaches */

        "mcr    p15, 0, r0, c7, c10, 4\n"   /* drain write buffer on v4 */
        "mcr    p15, 0, r0, c8, c7, 0\n"    /* ʹ��Чָ�����TLB */

        "mov    r4, %0\n"                   /* r4 = ҳ���ַ */
        "mcr    p15, 0, r4, c2, c0, 0\n"    /* ����ҳ���ַ�Ĵ��� */

        "mvn    r0, #0\n"
        "mcr    p15, 0, r0, c3, c0, 0\n"    /* ����ʿ��ƼĴ�����Ϊ0xFFFFFFFF��
                                             * ������Ȩ�޼��
                                             */
        /*
         * ���ڿ��ƼĴ������ȶ�����ֵ������������޸ĸ���Ȥ��λ��
         * Ȼ����д��
         */
        "mrc    p15, 0, r0, c1, c0, 0\n"    /* �������ƼĴ�����ֵ */

        /* ���ƼĴ����ĵ�16λ����Ϊ��.RVI ..RS B... .CAM
         * R : ��ʾ����Cache�е���Ŀʱʹ�õ��㷨��
         *     0 = Random replacement��1 = Round robin replacement
         * V : ��ʾ�쳣���������ڵ�λ�ã�
         *     0 = Low addresses = 0x00000000��1 = High addresses = 0xFFFF0000
         * I : 0 = �ر�ICaches��1 = ����ICaches
         * R��S : ������ҳ���е�������һ��ȷ���ڴ�ķ���Ȩ��
         * B : 0 = CPUΪС�ֽ���1 = CPUΪ���ֽ���
         * C : 0 = �ر�DCaches��1 = ����DCaches
         * A : 0 = ���ݷ���ʱ�����е�ַ�����飻1 = ���ݷ���ʱ���е�ַ������
         * M : 0 = �ر�MMU��1 = ����MMU
         */

        /*
         * ���������Ҫ��λ����������Ҫ��������������
         */
                                            /* .RVI ..RS B... .CAM */
        "bic    r0, r0, #0x3000\n"          /* ..11 .... .... .... ���V��Iλ */
        "bic    r0, r0, #0x0300\n"          /* .... ..11 .... .... ���R��Sλ */
        "bic    r0, r0, #0x0087\n"          /* .... .... 1... .111 ���B/C/A/M */

        /*
         * ������Ҫ��λ
         */
        "orr    r0, r0, #0x0002\n"          /* .... .... .... ..1. ���������� */
        "orr    r0, r0, #0x0004\n"          /* .... .... .... .1.. ����DCaches */
        "orr    r0, r0, #0x1000\n"          /* ...1 .... .... .... ����ICaches */
        "orr    r0, r0, #0x0001\n"          /* .... .... .... ...1 ʹ��MMU */

        "mcr    p15, 0, r0, c1, c0, 0\n"    /* ���޸ĵ�ֵд����ƼĴ��� */
        : /* ����� */
        : "r" (ttb)
    );
}
