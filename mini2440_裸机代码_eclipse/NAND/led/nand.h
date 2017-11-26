/*
 * nand.h
 *
 *  Created on: 2017��7��16��
 *      Author: Administrator
 */

#ifndef NAND_H_
#define NAND_H_


/* ���ⲿ���õĺ��� */
void nand_init(void);
void nand_read(unsigned char *buf, unsigned long start_addr, int size);
int nand_write(unsigned int nand_start, unsigned char * buf, unsigned int len);
void nand_erase_block(unsigned long addr);
#endif /* NAND_H_ */
