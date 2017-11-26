/*
 * io.h
 *
 *  Created on: 2017Äê7ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef BSP_IO_IO_H_
#define BSP_IO_IO_H_

#include "type.h"
#include "byteorder.h"

/*****************************************************************************/
#define read8       __raw_readb
#define read16      __raw_readw
#define read32      __raw_readl
#define read64      __raw_readll

#define write8      __raw_writeb
#define write16     __raw_writew
#define write32     __raw_writel
#define write64     __raw_writell


/*****************************************************************************/
static inline uint8_t __raw_readb(const volatile void  *addr)
{
    return *(const volatile uint8_t  *) addr;
}

static inline uint16_t __raw_readw(const volatile void  *addr)
{
    return *(const volatile uint16_t  *) addr;
}

static inline uint32_t __raw_readl(const volatile void  *addr)
{
    return *(const volatile uint32_t  *) addr;
}

static inline uint64_t __raw_readll(const volatile void  *addr)
{
    return *(const volatile uint64_t  *) addr;
}


#define readb           __raw_readb
#define readw(addr)     __le16_to_cpu(__raw_readw(addr))
#define readl(addr)     __le32_to_cpu(__raw_readl(addr))
#define readll(addr)    __le64_to_cpu(__raw_readll(addr))

static inline void __raw_writeb(uint8_t b, volatile void  *addr)
{
    *(volatile uint8_t  *) addr = b;
}

static inline void __raw_writew(uint16_t b, volatile void  *addr)
{
    *(volatile uint16_t  *) addr = b;
}

static inline void __raw_writel(uint32_t b, volatile void  *addr)
{
    *(volatile uint32_t  *) addr = b;
}

static inline void __raw_writell(uint64_t b, volatile void  *addr)
{
    *(volatile uint64_t  *) addr = b;
}

#define writeb          __raw_writeb
#define writew(b,addr)  __raw_writew(__cpu_to_le16(b),addr)
#define writel(b,addr)  __raw_writel(__cpu_to_le32(b),addr)
#define writell(b,addr) __raw_writell(__cpu_to_le64(b),addr)



#endif /* BSP_IO_IO_H_ */
