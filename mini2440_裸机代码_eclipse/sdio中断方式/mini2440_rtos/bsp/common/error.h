/*
 * error-base.h
 *
 *  Created on: 2017��7��26��
 *      Author: Administrator
 */

#ifndef BSP_COMMON_ERROR_H_
#define BSP_COMMON_ERROR_H_

#define EPERM           1   /* ���������� */
#define ENOENT          2   /* �ļ���Ŀ¼������ */
#define ESRCH           3   /* ���̲����� */
#define EINTR           4   /* ���ñ��ж� */
#define EIO             5   /* I/O ���� */
#define ENXIO           6   /* �豸���ַ������ */
#define E2BIG           7   /* �����б�̫�� */
#define ENOEXEC         8   /* ��ִ���ļ���ʽ���� */
#define EBADF           9   /* �ļ��������� */
#define ECHILD          10  /* û���ӽ��� */
#define EAGAIN          11  /* ��Դ�����ã������� */
#define ENOMEM          12  /* �ռ䣨�ڴ棩���� */
#define EACCES          13  /* Ȩ�޲��� */
#define EFAULT          14  /* ��ַ���� */
#define ENOTEMPTY       15  /* Ŀ¼�ǿ� */
#define EBUSY           16  /* �豸����Դæ */
#define EEXIST          17  /* �ļ��Ѿ����� */
#define EXDEV           18  /* ���豸���� */
#define ENODEV          19  /* �豸������ */
#define ENOTDIR         20  /* ����Ŀ¼ */
#define EISDIR          21  /* ��Ŀ¼ */
#define EINVAL          22  /* ��Ч���� */
#define ENFILE          23  /* ϵͳ���ļ�̫�࣬������������ */
#define EMFILE          24  /* �򿪵��ļ�̫�� */
#define ENOTTY          25  /* �����ʵ�I/O���Ʋ��� */
#define ENAMETOOLONG    26  /* �ļ���̫�� */
#define EFBIG           27  /* �ļ�̫�� */
#define ENOSPC          28  /* �豸ʣ��ռ䲻�� */
#define ESPIPE          29  /* ��Ч��������Invalid seek�� */
#define EROFS           30  /* �ļ�ϵͳֻ�� */
#define EMLINK          31  /* ����̫�� */
#define EPIPE           32  /* �𻵵Ĺܵ� */
#define EDEADLK         33  /* ��Դ�������� */
#define ENOLCK          34  /* �޿��ã����У����� */
#define ENOTSUP         35  /* ��֧�� */
#define EMSGSIZE        36  /* ��Ϣ̫�� */


#endif /* BSP_COMMON_ERROR_H_ */
