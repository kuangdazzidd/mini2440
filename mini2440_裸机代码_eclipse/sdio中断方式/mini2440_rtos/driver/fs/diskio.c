/*
 * File      : dfs_elm.c
 * This file is part of Device File System in RT-Thread RTOS
 * COPYRIGHT (C) 2008-2011, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-02-22     QiuYi        The first version.
 * 2011-10-08     Bernard      fixed the block size in statfs.
 * 2011-11-23     Bernard      fixed the rename issue.
 * 2012-07-26     aozima       implement ff_memalloc and ff_memfree.
 * 2012-12-19     Bernard      fixed the O_APPEND and lseek issue.
 * 2013-03-01     aozima       fixed the stat(st_mtime) issue.
 * 2014-01-26     Bernard      Check the sector size before mount.
 */

#include "ffconf.h"
#include "ff.h"
#include <string.h>
#include <time.h>
#include "diskio.h"
#include "sdio.h"


DSTATUS disk_initialize (BYTE pdrv)
{
    s3c2440_sd_init();
    return RES_OK;
}

DSTATUS disk_status (BYTE pdrv)
{
    return RES_OK;
}

DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    sdcard_read(sector, buff, count);
    return RES_OK;
}

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    sdcard_write(sector, buff, count);
    return RES_OK;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    struct sdcard *p_card = sdio_sdcard_get();

    switch(cmd) {
    case CTRL_SYNC:
        break;

    case GET_SECTOR_COUNT:
        /* Get drive capacity in unit of sector (DWORD) */
        *(DWORD *)buff = p_card->csd.nblks;
        break;

    case GET_SECTOR_SIZE:
        /* Get sector size (for (_MAX_SS >= 1024)) */
        *(WORD *)buff = p_card->csd.bsize;
        break;

    case GET_BLOCK_SIZE:
        /* Get erase block size in unit of sectors (DWORD)*/
        *(DWORD *)buff = 1;
        break;

    default:
        return RES_PARERR;
    }

    return RES_OK;
}

DWORD get_fattime (void)
{
    return 0;
}

