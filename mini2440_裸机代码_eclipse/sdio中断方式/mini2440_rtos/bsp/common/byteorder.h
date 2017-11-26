#ifndef __BYTEORDER_H__
#define __BYTEORDER_H__

#include <sys/_stdint.h>
#include <machine/endian.h>

static inline unsigned short __swab16(unsigned short x)
{
	return ( (x<<8) | (x>>8) );
}

static inline unsigned int __swab32(unsigned int x)
{
	return ( (x<<24) | (x>>24) | \
		((x & (unsigned int)0x0000ff00UL)<<8) | \
		((x & (unsigned int)0x00ff0000UL)>>8) );
}


static inline unsigned long long __swab64(unsigned long long x)
{
	return ( (x<<56) | (x>>56) | \
		((x & (unsigned long long)0x000000000000ff00ULL)<<40) | \
		((x & (unsigned long long)0x0000000000ff0000ULL)<<24) | \
		((x & (unsigned long long)0x00000000ff000000ULL)<< 8) | \
		((x & (unsigned long long)0x000000ff00000000ULL)>> 8) | \
		((x & (unsigned long long)0x0000ff0000000000ULL)>>24) | \
		((x & (unsigned long long)0x00ff000000000000ULL)>>40) );
}

/*
 * swap bytes bizarrely.
 * swahw32 - swap 16-bit half-words in a 32-bit word
 */
static inline unsigned int __swahw32(unsigned int x)
{
	return ( ((x & (unsigned int)0x0000ffffUL)<<16) | ((x & (unsigned int)0xffff0000UL)>>16) );
}

/*
 * swap bytes bizarrely.
 * swahb32 - swap 8-bit halves of each 16-bit half-word in a 32-bit word
 */
static inline unsigned int __swahb32(unsigned int x)
{
	return ( ((x & (unsigned int)0x00ff00ffUL)<<8) | ((x & (unsigned int)0xff00ff00UL)>>8) );
}

#if (BYTE_ORDER == BIG_ENDIAN)
#define cpu_to_le64(x)	(__swab64((unsigned long long)(x)))
#define le64_to_cpu(x)	(__swab64((unsigned long long)(x)))
#define cpu_to_le32(x)	(__swab32((unsigned int)(x)))
#define le32_to_cpu(x)	(__swab32((unsigned int)(x)))
#define cpu_to_le16(x)	(__swab16((unsigned short)(x)))
#define le16_to_cpu(x)	(__swab16((unsigned short)(x)))
#define cpu_to_be64(x)	((unsigned long long)(x))
#define be64_to_cpu(x)	((unsigned long long)(x))
#define cpu_to_be32(x)	((unsigned int)(x))
#define be32_to_cpu(x)	((unsigned int)(x))
#define cpu_to_be16(x)	((unsigned short)(x))
#define be16_to_cpu(x)	((unsigned short)(x))
#else
#define cpu_to_le64(x)	((unsigned long long)(x))
#define le64_to_cpu(x)	((unsigned long long)(x))
#define cpu_to_le32(x)	((unsigned int)(x))
#define le32_to_cpu(x)	((unsigned int)(x))
#define cpu_to_le16(x)	((unsigned short)(x))
#define le16_to_cpu(x)	((unsigned short)(x))
#define cpu_to_be64(x)	(__swab64((unsigned long long)(x)))
#define be64_to_cpu(x)	(__swab64((unsigned long long)(x)))
#define cpu_to_be32(x)	(__swab32((unsigned int)(x)))
#define be32_to_cpu(x)	(__swab32((unsigned int)(x)))
#define cpu_to_be16(x)	(__swab16((unsigned short)(x)))
#define be16_to_cpu(x)	(__swab16((unsigned short)(x)))
#endif


#define __le16_to_cpu   le16_to_cpu
#define __le32_to_cpu   le32_to_cpu
#define __le64_to_cpu   le64_to_cpu
#define __cpu_to_le16   cpu_to_le16
#define __cpu_to_le32   cpu_to_le32
#define __cpu_to_le64   cpu_to_le64
#define __be16_to_cpu   be16_to_cpu
#define __be32_to_cpu   be32_to_cpu
#define __be64_to_cpu   be64_to_cpu
#define __cpu_to_be16   cpu_to_be16
#define __cpu_to_be32   cpu_to_be32
#define __cpu_to_be64   cpu_to_be64


#endif /* __BYTEORDER_H__ */
