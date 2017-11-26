/*
 * sdio.c
 *
 *  Created on: 2017Äê8ÔÂ9ÈÕ
 *      Author: Administrator
 */

#include "clock.h"
#include "sdio.h"
#include "type.h"
#include "uart.h"
#include "string.h"
#include "gpio.h"

#define INICLK      300000
#define SDIOCLK     24000000    //PCLK=49.392MHz


volatile uint32_t rd_cnt;
volatile uint32_t wt_cnt;
volatile int32_t RCA;
volatile int32_t sd_type;

static void sd_delay(uint32_t ms)
{
//    ms *= 7326;
    ms *= 100;
    while(--ms);
}

static int sd_cmd_end(int cmd, int be_resp)
{
    int finish0;

    if (!be_resp)
    {
        finish0 = SDICSTA;

        while ((finish0&0x800) != 0x800)
            finish0 = SDICSTA;

        SDICSTA = finish0;

        return 0;
    }
    else
    {
        finish0 = SDICSTA;

        while (!(((finish0&0x200)==0x200) | ((finish0&0x400) == 0x400)))
            finish0=SDICSTA;

        if (cmd == 1 || cmd == 41)
        {
            if ((finish0 & 0xf00) != 0xa00)
            {
                SDICSTA = finish0;
                    if ((finish0&0x400) == 0x400)
                        return -1;
            }
            SDICSTA = finish0;
        }
        else
        {
            if ((finish0 & 0x1f00) != 0xa00)
            {
                /* ex_kprintf("CMD%d:SDICSTA=0x%x, SDIRSP0=0x%x\n", cmd, SDICSTA, SDIRSP0); */
                SDICSTA = finish0;
                if ((finish0 & 0x400) == 0x400)
                    return -1;
            }
            SDICSTA = finish0;
        }
        return 0;
    }
}

static int sd_data_end(void)
{
    int finish;

    finish = SDIDSTA;

    while (!(((finish & 0x10) == 0x10) | ((finish & 0x20) == 0x20)))
    {
        finish = SDIDSTA;
    }

    if ((finish & 0xfc) != 0x10)
    {
        SDIDSTA = 0xec;

        return -1;
    }

    return 0;
}

static void sd_cmd0(void)
{
    SDICARG = 0x0;
    SDICCON = (1<<8) | 0x40;

    sd_cmd_end(0, 0);
    SDICSTA = 0x800;      /* Clear cmd_end(no rsp) */
}

static int sd_cmd55(void)
{
    SDICARG = RCA << 16;
    SDICCON = (0x1 << 9) | (0x1 << 8) | 0x77;

    if (sd_cmd_end(55, 1) == -1)
    {
        /* ex_kprintf("CMD55 error\n"); */
        return -1;
    }

    SDICSTA = 0xa00;

    return 0;
}

static void sd_sel_desel(char sel_desel)
{
    if (sel_desel)
    {
RECMDS7:
        SDICARG = RCA << 16;
        SDICCON = (0x1 << 9) | (0x1 << 8) | 0x47;
        if (sd_cmd_end(7, 1) == -1)
            goto RECMDS7;

        SDICSTA = 0xa00;

        if (SDIRSP0 & 0x1e00 != 0x800)
            goto RECMDS7;
    }
    else
    {
RECMDD7:
        SDICARG = 0 << 16;
        SDICCON = (0x1 << 8) | 0x47;

        if (sd_cmd_end(7, 0) == -1)
            goto RECMDD7;
        SDICSTA = 0x800;
    }
}

static void sd_setbus(void)
{
    do
    {
        sd_cmd55();

        SDICARG = 1 << 1; /* 4bit bus */
        SDICCON = (0x1<<9) | (0x1<<8) | 0x46; /* sht_resp, wait_resp, start, CMD55 */
    }while (sd_cmd_end(6, 1) == -1);

    SDICSTA=0xa00;      /* Clear cmd_end(with rsp) */
}


static int sd_all_cid(uint32_t  *p_cid)
{

    return 0;
}

static int sd_ocr(void)
{
    int i, ver=0;

    /* Negotiate operating condition for SD, it makes card ready state */
    for (i = 0; i < 50; i ++)
    {
        sd_cmd55();

        SDICARG = 0x40ff8000; /* HCS=1, compatible v1.x and v2.0 */
        SDICCON = (0x1<<9) | (0x1<<8) | 0x69;

        /* if using real board, should replace code here. need to modify qemu in near future*/
        /* Check end of ACMD41 */
        if (sd_cmd_end(41, 1) == 0)
        {
            if (SDIRSP0 == 0x80ff8000)
            {
                ver = 1; /* SD V1.x, CCS=0 */
                break;
            }
            else if (SDIRSP0 == 0xc0ff8000)
            {
                ver = 2; /* SD V2.0, CCS=1 */
                break;
            }
        }

        sd_delay(200);
    }
    SDICSTA = 0xa00;

    return ver;
}

int sd_cmd8(void)
{
    SDICARG = 0x000001AA;
    SDICCON = (0x1<<9) | (0x1<<8) | 0x48; //sht_resp, wait_resp, start
    if (sd_cmd_end(8, 1) == -1)
        return -1;
    SDICSTA = 0xa00;

    if ((SDIRSP0&0x1aa) == 0x1aa)
        return 0;
    else
        return -1;
}

uint8_t s3c2440_sd_init()
{
    int i;

    CLKCON |= 1 << 9;

    /* Setup GPIO as SD and SDCMD, SDDAT[3:0] Pull up En */
    s3c2440_gpio_cfgpin(SDCLK, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDCMD, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT0, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT1, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT2, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT3, S3C2440_GPIO_SFN2);
    s3c2440_gpio_pullup(SDCLK, 1);


    RCA = 0;

    /* Important notice for MMC test condition */
    /* Cmd & Data lines must be enabled by pull up resister */
    SDIPRE    = PCLK / (INICLK) - 1;
    SDICON    = (0<<4) | 1;   // Type A, clk enable
    SDIFSTA   = SDIFSTA | (1<<16);
    SDIBSIZE  = 0x200;       /* 512byte per one block */
    SDIDTIMER = 0x7fffff;     /* timeout count */

    /* Wait 74SDCLK for MMC card */
    for (i = 0; i < 0x1000; i ++);

    sd_cmd0();
    sd_cmd8(); /* Must be use it, Host shall supports high capacity */

    /* Check SD card OCR */
    sd_type = sd_ocr();
    if (sd_type > 0)
    {
        ex_kprintf("In SD ready\n");
    }
    else
    {
        ex_kprintf("Initialize fail\nNo Card assertion\n");

        return -1;
    }

RECMD2:
    SDICARG = 0x0;
    SDICCON = (0x1<<10)|(0x1<<9)|(0x1<<8)|0x42; /* lng_resp, wait_resp, start, CMD2 */
    if (sd_cmd_end(2, 1) == -1)
        goto RECMD2;

    SDICSTA = 0xa00;    /* Clear cmd_end(with rsp) */

RECMD3:
    SDICARG = 0<<16;    /* CMD3(MMC:Set RCA, SD:Ask RCA-->SBZ) */
    SDICCON = (0x1<<9)|(0x1<<8)|0x43; /* sht_resp, wait_resp, start, CMD3 */
    if (sd_cmd_end(3, 1) == -1)
        goto RECMD3;
    SDICSTA=0xa00;  /* Clear cmd_end(with rsp) */

    RCA = (SDIRSP0 & 0xffff0000) >> 16;
    SDIPRE = PCLK / (SDIOCLK) - 1; /* Normal clock=25MHz */
    if (SDIRSP0 & 0x1e00 != 0x600)
        goto RECMD3;

    sd_sel_desel(1);
    sd_delay(200);
    sd_setbus();

    return 0;
}

uint8_t sd_readblock(uint32_t address, uint8_t *buf)
{
    uint32_t status, tmp;

    rd_cnt = 0;
    SDIFSTA = SDIFSTA | (1<<16);

    SDIDCON = (2 << 22) | (1 << 19) | (1 << 17) | (1 << 16) | (1 << 14) | (2 << 12) | (1 << 0);
    SDICARG = address;

RERDCMD:
    SDICCON = (0x1 << 9 ) | (0x1 << 8) | 0x51;
    if (sd_cmd_end(17, 1) == -1)
    {
        ex_kprintf("Read CMD Error\n");
        goto RERDCMD;
    }

    SDICSTA = 0xa00;

    while (rd_cnt < 128)
    {
        if ((SDIDSTA & 0x20) == 0x20)
        {
            SDIDSTA = (0x1 << 0x5);
            break;
        }
        status = SDIFSTA;
        if ((status & 0x1000) == 0x1000)
        {
            tmp = SDIDAT;
            memcpy(buf, &tmp, sizeof(uint32_t));
            rd_cnt ++;
            buf += 4;
        }
    }
    if (sd_data_end() == -1)
    {
        ex_kprintf("Dat error\n");

        return -1;
    }

    SDIDCON = SDIDCON &~ (7<<12);
    SDIFSTA = SDIFSTA & 0x200;
    SDIDSTA = 0x10;

    return 0;
}

static uint8_t sd_writeblock(uint32_t address, uint8_t *buf)
{
    uint32_t status, tmp;

    wt_cnt = 0;
    SDIFSTA = SDIFSTA | (1 << 16);

    SDIDCON = (2 << 22) | (1 << 20) | (1 << 17) | (1 << 16) | (1 << 14) | (3 << 12) | (1 << 0);
    SDICARG = address;

REWTCMD:
    SDICCON = (0x1 << 9) | (0x1 << 8) |0x58;

    if (sd_cmd_end(24, 1) == -1)
        goto REWTCMD;

    SDICSTA = 0xa00;

    while (wt_cnt < 128)
    {
        status = SDIFSTA;
        if ((status & 0x2000) == 0x2000)
        {
            memcpy(&tmp, buf, sizeof(uint32_t));
            SDIDAT = tmp;
            wt_cnt ++;
            buf += 4;
        }
    }
    if (sd_data_end() == -1)
    {
        ex_kprintf("Data Error\n");

        return -1;
    }
    SDIDCON = SDIDCON &~ (7<<12);
    SDIDSTA = 0x10;

    return 0;
}


#define SECTOR_SIZE              512

int sdcard_read(int    pos,
                void  *buffer,
                int    size)
{
    int i, addr;

    /* read all sectors */
    for (i = 0; i < size; i ++)
    {
        addr = (i + pos);
        sd_readblock(addr, (uint8_t *)((uint8_t *)buffer + i * SECTOR_SIZE));
    }

    /* the length of reading must align to SECTOR SIZE */
    return size;
}

int sdcard_write(int         pos,
                 const void *buffer,
                 int          size)
{
    int i, addr;

    /* read all sectors */
    for (i = 0; i < size; i++)
    {
        addr = (i + pos);
        sd_writeblock(addr, (uint8_t*)((uint8_t*)buffer + i * SECTOR_SIZE));
    }

    /* the length of reading must align to SECTOR SIZE */
    return size;
}



