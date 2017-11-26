/*
 * clock.h
 *
 *  Created on: 2017年7月3日
 *      Author: Administrator
 */

#ifndef CLOCK_H_
#define CLOCK_H_


/* 控制寄存器 */
#define CLKDIVN      (*(volatile unsigned long*)0x4C000014)
#define MPLLCON      (*(volatile unsigned long*)0x4C000004)

int clock_init();

#endif /* CLOCK_H_ */
