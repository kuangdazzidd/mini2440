/*
 * key.h
 *
 *  Created on: 2017Äê8ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef DRIVER_KEY_H_
#define DRIVER_KEY_H_

#include "gpio.h"

#define KEY1    S3C2440_GPG(0)    /* EINT8 */
#define KEY2    S3C2440_GPG(3)    /* EINT11 */
#define KEY3    S3C2440_GPG(5)    /* EINT13 */
#define KEY4    S3C2440_GPG(6)    /* EINT14 */
#define KEY5    S3C2440_GPG(7)    /* EINT15 */
#define KEY6    S3C2440_GPG(11)   /* EINT19 */


int key_init();


#endif /* DRIVER_KEY_H_ */
