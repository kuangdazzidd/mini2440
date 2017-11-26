/*
 * sdio.h
 *
 *  Created on: 2017Äê8ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef BSP_SDIO_SDIO_H_
#define BSP_SDIO_SDIO_H_

#include "type.h"

// SD Interface
#define SDICON     (*(volatile unsigned *)0x5a000000) //SDI control
#define SDIPRE     (*(volatile unsigned *)0x5a000004) //SDI baud ate prescaler
#define SDICARG    (*(volatile unsigned *)0x5a000008) //SDI command argument
#define SDICCON    (*(volatile unsigned *)0x5a00000c) //SDI command control
#define SDICSTA    (*(volatile unsigned *)0x5a000010) //SDI command status
#define SDIRSP0    (*(volatile unsigned *)0x5a000014) //SDI esponse 0
#define SDIRSP1    (*(volatile unsigned *)0x5a000018) //SDI esponse 1
#define SDIRSP2    (*(volatile unsigned *)0x5a00001c) //SDI esponse 2
#define SDIRSP3    (*(volatile unsigned *)0x5a000020) //SDI esponse 3
#define SDIDTIMER  (*(volatile unsigned *)0x5a000024) //SDI data/busy timer
#define SDIBSIZE   (*(volatile unsigned *)0x5a000028) //SDI block size
#define SDIDCON    (*(volatile unsigned *)0x5a00002c) //SDI data control
#define SDIDCNT    (*(volatile unsigned *)0x5a000030) //SDI data emain counter
#define SDIDSTA    (*(volatile unsigned *)0x5a000034) //SDI data status
#define SDIFSTA    (*(volatile unsigned *)0x5a000038) //SDI FIFO status
#define SDIIMSK    (*(volatile unsigned *)0x5a00003C) //SDI interrupt mask
#define SDIDAT     (*(volatile unsigned *)0x5a000040)



#define SDIO_RSP_NONE       0x0
#define SDIO_RSP_R1         0x1
#define SDIO_RSP_R1B        0x2
#define SDIO_RSP_R2         0x3
#define SDIO_RSP_R3         0x4
#define SDIO_RSP_R6         0x5
#define SDIO_RSP_R7         0x6


#define SDCLK       S3C2440_GPE(5)
#define SDCMD       S3C2440_GPE(6)
#define SDDAT0      S3C2440_GPE(7)
#define SDDAT1      S3C2440_GPE(8)
#define SDDAT2      S3C2440_GPE(9)
#define SDDAT3      S3C2440_GPE(10)



struct sd_cid {
    uint32_t                mid;
    uint16_t                oid;
    char                    pnm[8];
    uint8_t                 prv;
    uint32_t                psn;
    uint16_t                year;
    uint8_t                 month;
    uint8_t                 hwrev;
    uint8_t                 fwrev;
};

struct sd_csd {
    uint32_t    mspeed;
    uint16_t    bsize;
    uint32_t    nblks;
};


struct sdcard {
    uint32_t        rca;
    struct sd_cid   cid;
    struct sd_csd   csd;
};


uint8_t s3c2440_sd_init();

uint8_t sd_readblock(uint32_t address, uint8_t *buf);

int sdcard_read(int    pos,
               void  *buffer,
               int    size);

int sdcard_write(int         pos,
                 const void *buffer,
                 int          size);


int sdio_go_idle();
int sdio_send_if_cond();
int sdio_send_rca(uint32_t  *p_rca);
int sdio_app_op_cond(uint32_t    rca,
                     uint32_t    ocr,
                     uint32_t   *p_rocr);

int sdio_all_send_cid(uint32_t *p_cid);
int sdio_send_csd(uint32_t   rca,
                  uint32_t  *p_csd);
int sdio_select_card(uint32_t  rca);
int sdio_sd_switch();

struct sdcard *sdio_sdcard_get();

#endif /* BSP_SDIO_SDIO_H_ */
