/*
 * error-base.h
 *
 *  Created on: 2017年7月26日
 *      Author: Administrator
 */

#ifndef BSP_COMMON_ERROR_H_
#define BSP_COMMON_ERROR_H_

#define EPERM           1   /* 操作不允许 */
#define ENOENT          2   /* 文件或目录不存在 */
#define ESRCH           3   /* 进程不存在 */
#define EINTR           4   /* 调用被中断 */
#define EIO             5   /* I/O 错误 */
#define ENXIO           6   /* 设备或地址不存在 */
#define E2BIG           7   /* 参数列表太长 */
#define ENOEXEC         8   /* 可执行文件格式错误 */
#define EBADF           9   /* 文件描述符损坏 */
#define ECHILD          10  /* 没有子进程 */
#define EAGAIN          11  /* 资源不可用，需重试 */
#define ENOMEM          12  /* 空间（内存）不足 */
#define EACCES          13  /* 权限不够 */
#define EFAULT          14  /* 地址错误 */
#define ENOTEMPTY       15  /* 目录非空 */
#define EBUSY           16  /* 设备或资源忙 */
#define EEXIST          17  /* 文件已经存在 */
#define EXDEV           18  /* 跨设备连接 */
#define ENODEV          19  /* 设备不存在 */
#define ENOTDIR         20  /* 不是目录 */
#define EISDIR          21  /* 是目录 */
#define EINVAL          22  /* 无效参数 */
#define ENFILE          23  /* 系统打开文件太多，描述符不够用 */
#define EMFILE          24  /* 打开的文件太多 */
#define ENOTTY          25  /* 不合适的I/O控制操作 */
#define ENAMETOOLONG    26  /* 文件名太长 */
#define EFBIG           27  /* 文件太大 */
#define ENOSPC          28  /* 设备剩余空间不足 */
#define ESPIPE          29  /* 无效的搜索（Invalid seek） */
#define EROFS           30  /* 文件系统只读 */
#define EMLINK          31  /* 链接太多 */
#define EPIPE           32  /* 损坏的管道 */
#define EDEADLK         33  /* 资源可能死锁 */
#define ENOLCK          34  /* 无可用（空闲）的锁 */
#define ENOTSUP         35  /* 不支持 */
#define EMSGSIZE        36  /* 消息太大 */


#endif /* BSP_COMMON_ERROR_H_ */
