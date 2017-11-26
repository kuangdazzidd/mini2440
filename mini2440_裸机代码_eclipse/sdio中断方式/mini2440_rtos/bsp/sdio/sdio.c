/*
 * sdio.c
 *
 *  Created on: 2017年8月9日
 *      Author: Administrator
 */

#include "clock.h"
#include "sdio.h"
#include "type.h"
#include "uart.h"
#include "string.h"
#include "gpio.h"
#include "interrupt.h"

#define INICLK      300000
#define SDIOCLK     24000000    //PCLK=49.392MHz


#define __SDIO_MODE_INT_EN   1


static struct sdcard  __g_sdcard;

/******************************************************************************/
static int __send_cmd(uint8_t   cmd,
                      uint32_t  arg,
                      uint8_t   rsp_type,
                      void     *p_rsp);
static int __sd_decode_cid (struct sd_cid *p_cid_t, uint32_t *p_cid);
static int __sd_decode_csd (struct sd_csd       *p_csd_t,
                            uint32_t            *p_csd);

static void __irq_sdio_send();

/******************************************************************************/
static void __sd_delay(uint32_t ms)
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

static int sd_busy_end()
{
    int finish;

    finish=SDIDSTA;
    while( !( ((finish&0x08)==0x08) | ((finish&0x20)==0x20) ))
        finish=SDIDSTA;

    if( (finish&0xfc) != 0x08 )
    {
        //Uart_Printf("DATA:finish=0x%x\n", finish);
        SDIDSTA=0xf4;  //clear error state
        return 0;
    }
    return 1;
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

static int sd_cmd55(void)
{
    SDICARG = __g_sdcard.rca << 16;
    SDICCON = (0x1 << 9) | (0x1 << 8) | 0x77;

    if (sd_cmd_end(55, 1) == -1)
    {
        /* ex_kprintf("CMD55 error\n"); */
        return -1;
    }

    SDICSTA = 0xa00;

    return 0;
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


/******************************************************************************/
uint8_t s3c2440_sd_init()
{
    int i;
    int ret;
    uint32_t rsp[4];

    CLKCON |= 1 << 9;

    /* Setup GPIO as SD and SDCMD, SDDAT[3:0] Pull up En */
    s3c2440_gpio_cfgpin(SDCLK, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDCMD, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT0, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT1, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT2, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(SDDAT3, S3C2440_GPIO_SFN2);
    s3c2440_gpio_pullup(SDCLK, 1);

#if __SDIO_MODE_INT_EN
    /* 绑定中断回调函数 */
    s3c2440_interrupt_install_irq(INTSDI, __irq_sdio_send, NULL);
#endif

    memset(&__g_sdcard, 0x0, sizeof(__g_sdcard));

    /* Important notice for MMC test condition */
    /* Cmd & Data lines must be enabled by pull up resister */
    SDIPRE    = PCLK / (INICLK) - 1;
    SDICON    = (0<<4) | 1;   // Type A, clk enable
    SDIFSTA   = SDIFSTA | (1<<16);
    SDIBSIZE  = 0x200;       /* 512byte per one block */
    SDIDTIMER = 0x7fffff;     /* timeout count */

    /* Wait 74SDCLK for MMC card */
    for (i = 0; i < 0x100; i ++);

    uint32_t  ocr = 0x40ff8000;
    ret = sdio_go_idle();
    ret = sdio_send_if_cond();
    ret = sdio_app_op_cond(0, ocr, &ocr);

    uint32_t cid[4];
    ret = sdio_all_send_cid(cid);
    if (ret == 0) {
        __sd_decode_cid(&__g_sdcard.cid, cid);
    }


RECMD3:
    ret = sdio_send_rca(&__g_sdcard.rca);
    SDIPRE = PCLK / (SDIOCLK) - 1; /* Normal clock=25MHz */
    if (SDIRSP0 & 0x1e00 != 0x600)
        goto RECMD3;

    uint32_t csd[4];
    ret = sdio_send_csd(__g_sdcard.rca, csd);
    ret = __sd_decode_csd(&__g_sdcard.csd, csd);

    ret = sdio_select_card(__g_sdcard.rca);

    __sd_delay(200);
    sd_setbus();

    return 0;
}

uint8_t sd_readblock(uint32_t address, uint8_t *buf)
{
    uint32_t status, tmp;
    uint32_t rd_cnt;
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


uint8_t  g_tr_end   = 0;
uint8_t  g_tr_block = 0;
uint32_t g_tr_cnt   = 0;
uint8_t *gp_tr_buf  = NULL;

static void __irq_sdio_send()
{
    if (g_tr_cnt <= 128 * g_tr_block) {
        SDIDAT = *((uint32_t*)(&gp_tr_buf[g_tr_cnt]));
        if (*((uint32_t*)(&gp_tr_buf[g_tr_cnt])) != 0x45454545) {
            g_tr_cnt = g_tr_cnt;
        }
    }

    g_tr_cnt += 4;

    if(g_tr_cnt >= 128 * g_tr_block)
    {
        s3c2440_interrupt_mask(INTSDI);
        g_tr_end = 1;
    }
}


static uint8_t sd_write_multiblock(uint32_t address, uint32_t block, uint8_t *buf)
{
    uint32_t status, tmp;
    uint32_t wt_cnt;
    int ret;
    uint32_t rsp[4];

    wt_cnt = 0;
    SDIFSTA = SDIFSTA | (1 << 16);
    SDIDCON = (2 << 22) | (1 << 20) | (1 << 17) | (1 << 16) | (1 << 14) | (3 << 12) | (block << 0);

#define MODE_POL 0
#define MODE_INT 1
#define MODE_DMA 2
    uint8_t mode = MODE_INT;
    switch (mode) {
    case MODE_POL:
        ret = __send_cmd(25, address, SDIO_RSP_R1, &rsp);
        if (ret != 0) {
            ex_kprintf("Data trans Error\n");
            return -1;
        }
        while (wt_cnt < 128 * block)
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
        break;

    case MODE_INT:
        g_tr_cnt   = 0;
        gp_tr_buf  = buf;
        g_tr_block = block;
        /* 取消屏蔽 */
        s3c2440_interrupt_unmask(INTSDI);
        SDIIMSK = 0x10;  // Tx FIFO half int.

        ret = __send_cmd(25, address, SDIO_RSP_R1, &rsp);
        if (ret != 0) {
          ex_kprintf("Data trans Error\n");
          return -1;
        }
        while(!g_tr_end);

        s3c2440_interrupt_mask(INTSDI);
        g_tr_end = 0;
        SDIIMSK  = 0; // All mask
        break;

    case MODE_DMA:
        break;
    default :
        break;
    }


    /* check data send end */
//    if (sd_data_end() == -1)
//    {
//        ex_kprintf("Data Error\n");
//        return -1;
//    }
    /* Clear Data Transfer mode => no operation, Cleata Data Transfer start */
    SDIDCON = SDIDCON &~ (7<<12);
    /* Clear data Tx/Rx end */
    SDIDSTA = 0x10;

    if (block > 1) {
        SDIDCON=(1<<18)|(1<<17)|(0<<16)|(1<<14)|(1<<12)|(block<<0);

        /* CMD12 STOP TRANS */
        ret = __send_cmd(12, address, SDIO_RSP_R1, &rsp);
        if (ret != 0) {
            ex_kprintf("Data trans Error\n");
            return -1;
        }

        sd_busy_end();
        SDIDSTA = 0x08;
    }

    return 0;
}

static uint8_t sd_writeblock(uint32_t address, uint8_t *buf)
{
    uint32_t status, tmp;
    uint32_t wt_cnt;
    int ret;
    uint32_t rsp[4];

    wt_cnt = 0;

    /* DATA FIFO RESET */
    SDIFSTA = SDIFSTA | (1 << 16);

    /* 22(word transfer) | 20(Transmit After Response) | 17(Block data transfer)
     * 16(Wide bus mode) | 14(Data Transfer Start)     | 12(Data transmit mode)
     * 0 (BlkNum)
     */
    SDIDCON = (2 << 22) | (1 << 20) | (1 << 17) | (1 << 16) | (1 << 14) | (3 << 12) | (1 << 0);

    ret = __send_cmd(24, address, SDIO_RSP_R1, &rsp);
    if (ret != 0) {
        ex_kprintf("Data trans Error\n");
        return -1;
    }

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
    if (size > 1) {
        sd_write_multiblock(pos, size, (uint8_t*)buffer);
    } else {
        for (i = 0; i < size; i++)
        {
            addr = (i + pos);
            sd_writeblock(addr, (uint8_t*)((uint8_t*)buffer + i * SECTOR_SIZE));
        }
    }

    /* the length of reading must align to SECTOR SIZE */
    return size;
}



static int __send_cmd(uint8_t   cmd,
                      uint32_t  arg,
                      uint8_t   rsp_type,
                      void     *p_rsp)
{
    int finish0;

    SDICARG = arg;

    switch (rsp_type) {
    case SDIO_RSP_NONE:
        SDICCON = (0x1<<8) | (0x40 | cmd);
        break;
    case SDIO_RSP_R1:
    case SDIO_RSP_R1B:
    case SDIO_RSP_R3:
    case SDIO_RSP_R6:
    case SDIO_RSP_R7:
        SDICCON = (1 << 9) | (0x1<<8) | (0x40 | cmd);
        break;
    case SDIO_RSP_R2:
        SDICCON = (1 << 10) | (1 << 9) | (0x1<<8) | (0x40 | cmd);
        break;
    default :
        return -EINVAL;

    }

    /* 不需要响应 */
    if (rsp_type == SDIO_RSP_NONE) {
        finish0 = SDICSTA;
        while (!(finish0 & (1 << 11))) {
            finish0 = SDICSTA;
        }
        SDICSTA = finish0;
        return 0;
    } else {
        finish0 = SDICSTA;
        while (!(finish0 & (1 << 9)) && !(finish0 & (1 << 10))) {
            finish0 = SDICSTA;
        }
        /* TIMEOUT */
        if (finish0 & (1 << 10)) {
            SDICSTA = 0xa00;
        }
        SDICSTA = finish0;
    }

    if (rsp_type == SDIO_RSP_R2) {
        ((uint32_t *)p_rsp)[0] = SDIRSP3;
        ((uint32_t *)p_rsp)[1] = SDIRSP2;
        ((uint32_t *)p_rsp)[2] = SDIRSP1;
        ((uint32_t *)p_rsp)[3] = SDIRSP0;
    } else {
        ((uint32_t *)p_rsp)[0] = SDIRSP0;
    }

    return 0;
}


int sdio_go_idle()
{
    int ret;
    ret = __send_cmd(0, 0, SDIO_RSP_NONE, NULL);
    return ret;
}



int sdio_send_if_cond()
{
    int      ret;
    uint32_t rsp[4];
    ret = __send_cmd(8, 0x000001AA, SDIO_RSP_R7, (void*)&rsp);
    if (rsp[0] & 0x1aa) {
        ex_kprintf("Voltage ok\n");
        return 0;
    }
    return ret;
}


int sdio_send_rca(uint32_t  *p_rca)
{
    int      ret;
    uint32_t rsp[4];
    ret = __send_cmd(3, 0, SDIO_RSP_R6, (void*)&rsp);
    if (ret == 0) {
        *p_rca = rsp[0]>>16;
    }

    return ret;
}

int sdio_app_op_cond(uint32_t    rca,
                     uint32_t    ocr,
                     uint32_t   *p_rocr)
{
    int      ret;
    int      ver;
    uint32_t rsp[4];
    int      rty = 10;

    while (rty-- > 0) {
        ret = __send_cmd(55, (rca << 16), SDIO_RSP_R1, (void*)&rsp);
        if (ret != 0) {
            return ret;
        }

        ret = __send_cmd(41, ocr, SDIO_RSP_R3, (void*)&rsp);
        if (ret != 0) {
            return ret;
        }

        *p_rocr = rsp[0];

        if (*p_rocr == 0x80ff8000) {
            ver = 1;
            break;
        } else if (*p_rocr == 0xc0ff8000) {
            ver = 2;
            break;
        }

        __sd_delay(200);
    }

    return ret;
}


int sdio_all_send_cid(uint32_t *p_cid)
{
    int      ret;
    uint32_t rsp[4];
    ret = __send_cmd(2, 0, SDIO_RSP_R2, (void*)&rsp);
    if (ret != 0) {
        return ret;
    }

    memcpy(p_cid, rsp, 16);
    return ret;
}


static uint32_t __get_bits_str (uint32_t *str, uint32_t start, uint8_t  len)
{
    uint32_t  mask;
    uint32_t  index;
    uint8_t   shift;
    uint32_t  value;

    mask  = (int)((len < 32) ? (1 << len) : 0) - 1;
    index = start / 32;
    shift = start & 31;
    value = str[index] >> shift;

    if ((len + shift) > 32) {
        value |= str[index + 1] << (32 - shift);
    }
    value &= mask;
    return value;
}


static int __sd_decode_cid (struct sd_cid *p_cid_t, uint32_t *p_cid)
{
    p_cid_t->mid    = __get_bits_str(p_cid, 120, 8);
    p_cid_t->oid    = __get_bits_str(p_cid, 104, 16);
    p_cid_t->pnm[0] = __get_bits_str(p_cid, 96, 8);
    p_cid_t->pnm[1] = __get_bits_str(p_cid, 88, 8);
    p_cid_t->pnm[2] = __get_bits_str(p_cid, 80, 8);
    p_cid_t->pnm[3] = __get_bits_str(p_cid, 72, 8);
    p_cid_t->pnm[4] = __get_bits_str(p_cid, 64, 8);
    p_cid_t->prv    = __get_bits_str(p_cid, 56, 8);
    p_cid_t->psn    = __get_bits_str(p_cid, 24, 32);
    p_cid_t->year   = __get_bits_str(p_cid, 12, 8) + 2000;
    p_cid_t->month  = __get_bits_str(p_cid, 8, 4);
    p_cid_t->hwrev  = 0;
    p_cid_t->fwrev  = 0;
    return 0;
}


static const uint32_t __g_tran_exp[] = {
    10000, 100000, 1000000, 10000000, 0, 0, 0, 0
};

static const uint8_t __g_tran_mant[] = {
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

static int __sd_decode_csd (struct sd_csd       *p_csd_t,
                            uint32_t            *p_csd)
{
    uint32_t e, m, r;
    uint8_t  structure;

    structure = __get_bits_str(p_csd, 126, 2);

    switch (structure) {
        case 0:
            m = __get_bits_str(p_csd, 99, 4);
            e = __get_bits_str(p_csd, 96, 3);
            p_csd_t->mspeed = __g_tran_exp[e] * __g_tran_mant[m];
            p_csd_t->bsize  = 512;
            m = __get_bits_str(p_csd, 62, 12);
            e = __get_bits_str(p_csd, 47, 3);
            r = __get_bits_str(p_csd, 80, 4);
            p_csd_t->nblks = ((1 + m) << (e + r - 7));
            break;

        case 1:
            m = __get_bits_str(p_csd, 99, 4);
            e = __get_bits_str(p_csd, 96, 3);
            p_csd_t->mspeed = __g_tran_exp[e] * __g_tran_mant[m];
            p_csd_t->bsize  = 512;
            m = __get_bits_str(p_csd, 48, 22);
            p_csd_t->nblks = (1 + m) << 10;
            break;

        default:
            return -EINVAL;
    }
    return 0;
}



int sdio_send_csd(uint32_t   rca,
                  uint32_t  *p_csd)
{
    int      ret;
    uint32_t rsp[4];
    ret = __send_cmd(9, (rca << 16), SDIO_RSP_R2, (void*)&rsp);
    if (ret != 0) {
        return ret;
    }

    memcpy(p_csd, rsp, 16);
    return ret;
}


int sdio_select_card(uint32_t  rca)
{
    int      ret;
    uint32_t rsp[4];
    ret = __send_cmd(7, (rca << 16), SDIO_RSP_R1, (void*)&rsp);
    return ret;
}


int sdio_sd_switch()
{
    int ret;
    return ret;
}


struct sdcard *sdio_sdcard_get()
{
    return &__g_sdcard;
}
