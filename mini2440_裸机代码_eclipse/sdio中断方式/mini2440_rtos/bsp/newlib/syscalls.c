#include <reent.h>
#include <time.h>
#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"
#include "error.h"

/* Reentrant versions of system calls.  */

int
_close_r(struct _reent *ptr, int fd)
{
//	return close(fd);
    ptr->_errno = ENOTSUP;
    return -1;
}

int
_execve_r(struct _reent *ptr, const char * name, char *const *argv, char *const *env)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_fork_r(struct _reent *ptr)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_getpid_r(struct _reent *ptr)
{
	return 0;
}

int
_isatty_r(struct _reent *ptr, int fd)
{
	if (fd >=0 && fd < 3) return 1;

	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_kill_r(struct _reent *ptr, int pid, int sig)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_link_r(struct _reent *ptr, const char *old, const char *new)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

_off_t
_lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
//	return lseek(fd, pos, whence);
    ptr->_errno = ENOTSUP;
    return -1;
}

int
_mkdir_r(struct _reent *ptr, const char *name, int mode)
{
//	return mkdir(name, mode);
    ptr->_errno = ENOTSUP;
    return -1;
}

int
_open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    ptr->_errno = ENOTSUP;
    return -1;
//	return open(file, flags, mode);
}

_ssize_t 
_read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    if (fd < 3) {
//        return aw_serial_read(COM0, buf, nbytes);
    }
//	return read(fd, buf, nbytes);
    ptr->_errno = ENOTSUP;
    return -1;
}

int
_rename_r(struct _reent *ptr, const char *old, const char *new)
{
//	return rename(old, new);
    ptr->_errno = ENOTSUP;
    return -1;
}

void *
_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
	/* no use this routine to get memory */
	return NULL;
}

int
_stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
	return stat(file, pstat);
}

_CLOCK_T_
_times_r(struct _reent *ptr, struct tms *ptms)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_unlink_r(struct _reent *ptr, const char *file)
{
//	return unlink(file);
    ptr->_errno = ENOTSUP;
    return -1;
}

int
_wait_r(struct _reent *ptr, int *status)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

_ssize_t
_write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
//	if (fd < 3)	{
//	    return aw_serial_write(COM0, buf, nbytes);
//	} else	{
//	    return write(fd, buf, nbytes);
//	}
    ptr->_errno = ENOTSUP;
    return -1;
}

int
_gettimeofday_r(struct _reent *ptr, struct timeval *__tp, void *__tzp)
{
//	struct aw_timespec tp;
//
//	if (aw_timespec_get(&tp) == 0)
//	{
//		if (__tp != NULL)
//		{
//			__tp->tv_sec  = tp.tv_sec;
//			__tp->tv_usec = tp.tv_nsec * 1000UL;
//		}
//
//		return tp.tv_sec;
//	}

	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

/* Memory routine */
void *
_malloc_r (struct _reent *ptr, size_t size)
{
	void* result;

//	result = (void*)aw_mem_alloc (size);
	if (result == NULL)
	{
		ptr->_errno = ENOMEM;
	}

	return result;
}

void *
_realloc_r (struct _reent *ptr, void *old, size_t newlen)
{
	void* result;

//	result = (void*)aw_mem_realloc (old, newlen);
	if (result == NULL)
	{
		ptr->_errno = ENOMEM;
	}

	return result;
}

void *_calloc_r (struct _reent *ptr, size_t size, size_t len)
{
	void* result;

//	result = (void*)aw_mem_calloc (size, len);
	if (result == NULL)
	{
		ptr->_errno = ENOMEM;
	}

	return result;
}

void 
_free_r (struct _reent *ptr, void *addr)
{
//	aw_mem_free (addr);
}

void
_exit (int status)
{
#ifdef RT_USING_MODULE
	rt_module_t module;

	module = rt_module_self();
	if (module != RT_NULL)
	{
		struct rt_list_node *list;
		struct rt_object *object;

		rt_enter_critical();
		
        /* delete all threads in the module */
        list = &module->module_object[RT_Object_Class_Thread].object_list;
        while (list->next != list)
        {
            object = rt_list_entry(list->next, struct rt_object, list);
            if (rt_object_is_systemobject(object) == RT_TRUE)
            {
                /* detach static object */
                rt_thread_detach((rt_thread_t)object);
            }
            else
            {
                /* delete dynamic object */
                rt_thread_delete((rt_thread_t)object);
            }
        }
		/* delete main thread */
		rt_thread_delete(module->module_thread);
		rt_exit_critical();

		/* re-schedule */
		rt_schedule();
	}
#endif
	
//	rt_kprintf("thread:%s exit with %d\n", rt_thread_self()->name, status);

	while (1);
}

void 
_system(const char *s)
{
    /* not support this call */
    return;
}
