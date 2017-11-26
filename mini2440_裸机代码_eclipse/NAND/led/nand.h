/*
 * nand.h
 *
 *  Created on: 2017年7月16日
 *      Author: Administrator
 */

#ifndef NAND_H_
#define NAND_H_


/* 供外部调用的函数 */
void nand_init(void);
void nand_read(unsigned char *buf, unsigned long start_addr, int size);
int nand_write(unsigned int nand_start, unsigned char * buf, unsigned int len);
void nand_erase_block(unsigned long addr);
#endif /* NAND_H_ */
