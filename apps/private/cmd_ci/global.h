#ifndef _GLOBAL_H_
#define _GLOBAL_H_

typedef struct {
    char *name;
    int (*func)(int argc,char *argv[],void *p);
    int flags;
    int argcmin;
    char *argc_errmsg;
} cmds_t;

#define writew_lock(fd,offset,whence,len) \
		lock_reg(fd,F_SETLKW,F_WRLCK,offset,whence,len)

#define un_lock(fd,offset,whence,len) \
		lock_reg(fd,F_SETLK,F_UNLCK,offset,whence,len)
		
static int lock_reg(int fd,int cmd,int type,off_t offset,int whence,int len)
{
	struct flock lock;
	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;
	return (fcntl(fd,cmd,&lock));
}

#endif
