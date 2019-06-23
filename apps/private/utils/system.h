#ifndef _UTILS_SYSTEM_H
#define _UTILS_SYSTEM_H

#ifdef TCSUPPORT_ADD_JFFS
	#ifndef FS_MOUNT_PATH
	#define FS_MOUNT_PATH "/jffs/"
	#else
	#error DUPLICATE_FS_MOUNT_PATH
	#endif
#endif

#ifdef TCSUPPORT_SQUASHFS_ADD_YAFFS
	#ifndef FS_MOUNT_PATH
	#define FS_MOUNT_PATH "/yaffs/"
	#else
	#error DUPLICATE_FS_MOUNT_PATH
	#endif
#endif

#ifndef FS_MOUNT_PATH
#define FS_MOUNT_PATH "";
#endif

//path to keep data even after reset to default
#define RESERVE_DIR_NAME ".reserve"
#define RESERVE_PATH	FS_MOUNT_PATH "/" RESERVE_DIR_NAME "/"

#define USER_ROMFILE_PATH RESERVE_PATH "user_romfile"

#define TMP_DIR "/tmp/"
#define RUN_ROMFILE_DIR "/tmp/var/"
#define RUN_ROMFILE_NAME "romfile.cfg"
#define RUN_ROMFILE_PATH RUN_ROMFILE_DIR RUN_ROMFILE_NAME

#define USER_ROMFILE_SIZE 0x10000
#define USER_ROMFILE_OFFSET 0x10000

extern int mtd_getinfo(const char *mtdname, int *part, int *size);
extern int backup_userromfile();
extern int remove_userromfile();
#endif
