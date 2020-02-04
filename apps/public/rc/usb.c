/*

	USB Support

*/
#include "rc.h"

// #ifdef RTCONFIG_USB
#ifdef TCSUPPORT_USBHOST
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/mount.h>
#include <mntent.h>
#include <dirent.h>
#include <sys/file.h>
#include <sys/swap.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <rc_event.h>
#include <shared.h>

#include <usb_info.h>
#include <disk_io_tools.h>
#include <disk_share.h>
#include <disk_initial.h>

#include <bin_sem_asus.h>

char *usb_dev_file = "/proc/bus/usb/devices";

#define USB_DATA	"USB_Entry"
#define WEBDAV		"AiCloud_Entry"
#define CLOUDSYNC	"AiCloud_Entry"
#define APPS_DATA	"Apps_Entry"
#define SAMBA_DATA	"Samba_Entry"

#ifdef TCSUPPORT_SWAP_FILE
#define SWAP_FILENAME		"swap.sys"
#define SWAP_FOLDER		"asus_swap"
#define	SWAP_DD_COMMAND		"/bin/dd if=/dev/zero of=%s/asus_swap/swap.sys bs=%ld count=%ld conv=sync"
#define SWAP_FILE_SIZE_64M	67108864
#define SWAP_FILE_SIZE_128M	134217728
#define SWAP_FILE_SIZE_256M	268435456
#define USB_DISK_SIZE_1G	900
#define USB_DISK_SIZE_2G	1800
#define USB_DISK_SIZE_4G	3600
#define USB_DISK_SIZE_64G	57600
int create_swapfile(char *mountpoint);
int using_swap(void);
void start_swapfile(char *mountpoint);
void stop_swapfile(char *mountpoint);
#endif

#define USB_CLS_PER_INTERFACE	0	/* for DeviceClass */
#define USB_CLS_AUDIO		1
#define USB_CLS_COMM		2
#define USB_CLS_HID		3
#define USB_CLS_PHYSICAL	5
#define USB_CLS_STILL_IMAGE	6
#define USB_CLS_PRINTER		7
#define USB_CLS_MASS_STORAGE	8
#define USB_CLS_HUB		9
#define USB_CLS_CDC_DATA	0x0a
#define USB_CLS_CSCID		0x0b	/* chip+ smart card */
#define USB_CLS_CONTENT_SEC	0x0d	/* content security */
#define USB_CLS_VIDEO		0x0e
#define USB_CLS_WIRELESS_CONTROLLER	0xe0
#define USB_CLS_MISC		0xef
#define USB_CLS_APP_SPEC	0xfe
#define USB_CLS_VENDOR_SPEC	0xff
#define USB_CLS_3GDEV		0x35

#define OP_MOUNT		1
#define OP_UMOUNT		2
#define OP_SETNVRAM		3

char *find_sddev_by_mountpoint(char *mountpoint);

/* Adjust bdflush parameters.
 * Do this here, because Tomato doesn't have the sysctl command.
 * With these values, a disk block should be written to disk within 2 seconds.
 */
#ifdef LINUX26
void tune_bdflush(void)
{
	f_write_string("/proc/sys/vm/dirty_expire_centisecs", "200", 0, 0);
	f_write_string("/proc/sys/vm/dirty_writeback_centisecs", "200", 0, 0);
}
#else
#include <sys/kdaemon.h>
#define SET_PARM(n) (n * 2 | 1)
void tune_bdflush(void)
{
	bdflush(SET_PARM(5), 100);
	bdflush(SET_PARM(6), 100);
	bdflush(SET_PARM(8), 0);
}
#endif // LINUX26

#define USBCORE_MOD	"usbcore"
#define USB20_MOD	"ehci-hcd"
#define USBSTORAGE_MOD	"usb-storage"
#define SCSI_MOD	"scsi_mod"
#define SD_MOD		"sd_mod"
#define SG_MOD		"sg"
#ifdef LINUX26
#define USBOHCI_MOD	"ohci-hcd"
#define USBUHCI_MOD	"uhci-hcd"
#define USBPRINTER_MOD	"usblp"
#define SCSI_WAIT_MOD	"scsi_wait_scan"
#define USBFS		"usbfs"
#else
#define USBOHCI_MOD	"usb-ohci"
#define USBUHCI_MOD	"usb-uhci"
#define USBPRINTER_MOD	"printer"
#define USBFS		"usbdevfs"
#endif

// #ifdef RTCONFIG_USB_PRINTER
#ifdef TCSUPPORT_USB_PRINTER_SERVER
void
start_lpd()
{
	// if(getpid()!=1) {
		// notify_rc("start_lpd");
		// return;
	// }

	if (!pids("lpd"))
	{
		unlink("/var/run/lpdparent.pid");
		//return xstart("lpd");
		system("/userfs/bin/lpd &");
	}
}

void
stop_lpd()
{
	// if(getpid()!=1) {
		// notify_rc("stop_lpd");
		// return;
	// }

	if (pids("lpd"))
	{
		killall_tk("lpd");
		unlink("/var/run/lpdparent.pid");
	}
}

void
start_u2ec()
{
	// if(getpid()!=1) {
		// notify_rc("start_u2ec");
		// return;
	// }

	if (!pids("u2ec"))
	{
		unlink("/var/run/u2ec.pid");
		// system("u2ec &");
		system("/userfs/bin/u2ec &");

		// nvram_set("apps_u2ec_ex", "1");
		tcapi_set(USB_DATA, "apps_u2ec_ex", "1");
	}
}

void
stop_u2ec()
{
	// if(getpid()!=1) {
		// notify_rc("stop_u2ec");
		// return;
	// }

	if (pids("u2ec")){
		//system("killall -SIGKILL u2ec");
		killall_tk("u2ec");
		unlink("/var/run/u2ec.pid");
	}
}
#endif

FILE* fopen_or_warn(const char *path, const char *mode)
{
	FILE *fp = fopen(path, mode);

	if (!fp)
	{
		dbg("hotplug USB: No such file or directory: %s\n", path);
		errno = 0;
		return NULL;
	}

	return fp;
}

#ifdef DLM

int
test_user(char *target, char *pattern)
{
	char s[384];
	char p[32];
	char *start;
	char *pp;
	strcpy(s, target);
	strcpy(p, pattern);
	start = s;
	while (pp=strchr(start, ';'))
	{
		*pp='\0';
		if (! strcmp(start, p))
			return 1;
		start=pp+1;
	}
	return 0;
}

#define MOUNT_VAL_FAIL 	0
#define MOUNT_VAL_RONLY	1
#define MOUNT_VAL_RW 	2

int
fill_smbpasswd_input_file(const char *passwd)
{
	FILE *fp;
	
	unlink("/tmp/smbpasswd");
	fp = fopen("/tmp/smbpasswd", "w");
	
	if (fp && passwd)
	{
		fprintf(fp,"%s\n", passwd);
		fprintf(fp,"%s\n", passwd);
		fclose(fp);
		
		return 1;
	}
	else
		return 0;
}
#endif



void start_usb(void)
{
	char param[32];
	// int i;

	_dprintf("%s\n", __FUNCTION__);

	tune_bdflush();

	// if (nvram_get_int("usb_enable")) {
		modprobe(USBCORE_MOD);

		/* mount usb device filesystem */
        	mount(USBFS, "/proc/bus/usb", USBFS, MS_MGC_VAL, NULL);

// #ifdef LINUX26
		// if ((i = nvram_get_int("led_usb_gpio")) != 255) {
			// modprobe("ledtrig-usbdev");
			// modprobe("leds-usb");
			// sprintf(param, "%d", i);
			// f_write_string("/sys/class/leds/usb-led/gpio_pin", param, 0, 0);
			// f_write_string("/sys/class/leds/usb-led/device_name", "1-1", 0, 0);
		// }
// #endif

#if defined(RTCONFIG_SAMBASRV) || defined(RTCONFIG_FTP)
		// if (nvram_get_int("usb_storage")) {
			/* insert scsi and storage modules before usb drivers */
			modprobe(SCSI_MOD);
#ifdef LINUX26
			modprobe(SCSI_WAIT_MOD);
#endif
			modprobe(SD_MOD);
			modprobe(SG_MOD);
			modprobe(USBSTORAGE_MOD);

			// if (nvram_get_int("usb_fs_ext3")) {
#ifdef LINUX26
				modprobe("mbcache");	// used by ext2/ext3
#endif
				/* insert ext3 first so that lazy mount tries ext3 before ext2 */
				modprobe("jbd");
				modprobe("ext3");
				modprobe("ext2");
			// }

			// if (nvram_get_int("usb_fs_fat")) {
				// modprobe("fat");
				// modprobe("vfat");
			// }

#ifdef TCSUPPORT_USB_NTFS_UFSD
			modprobe("ufsd");
#endif
#ifdef TCSUPPORT_USB_NTFS
			modprobe("fuse");
#endif

		// }
#endif
		modprobe("nls_utf8");
		modprobe("nls_cp936");
		modprobe("nls_cp950");

		/* if enabled, force USB2 before USB1.1 */
		// if (nvram_get_int("usb_usb2") == 1) {
			// i = nvram_get_int("usb_irq_thresh");
			//if ((i < 0) || (i > 6))
				//i = 0;
			sprintf(param, "log2_irq_thresh=%d", 0);
			modprobe(USB20_MOD, param);
		// }

		// if (nvram_get_int("usb_uhci") == 1) {
			// modprobe(USBUHCI_MOD);
		// }

		// if (nvram_get_int("usb_ohci") == 1) {
			modprobe(USBOHCI_MOD);
		// }
// #ifdef RTCONFIG_USB_PRINTER
#ifdef TCSUPPORT_USB_PRINTER_SERVER
		// if (nvram_get_int("usb_printer")) {
			symlink("/dev/usb", "/dev/printers");
			modprobe(USBPRINTER_MOD);
			start_u2ec();
			start_lpd();
		// }
#endif
#ifdef RTCONFIG_USB_MODEM
		modprobe("usbnet");
		modprobe("asix");
		modprobe("cdc_ether");
		modprobe("rndis_host");
		modprobe("cdc_ncm");
		modprobe("cdc_wdm");
		if(nvram_match("modem_enable_qmi", "1"))
			modprobe("qmi_wwan");
		modprobe("cdc_mbim");
#endif
	// }
}

void remove_usb_modem_modules(void)
{
#ifdef RTCONFIG_USB_MODEM
#ifdef RTCONFIG_USB_BECEEM
	modprobe_r("drxvi314");
#endif
	modprobe_r("cdc_mbim");
	if(nvram_match("modem_enable_qmi", "1"))
		modprobe_r("qmi_wwan");
	modprobe_r("cdc_wdm");
	modprobe_r("cdc_ncm");
	modprobe_r("rndis_host");
	modprobe_r("cdc_ether");
	modprobe_r("asix");
	modprobe_r("usbnet");
#endif
}

void stop_usb(void)
{
	int disabled = 1;//!nvram_get_int("usb_enable");

#ifdef RTCONFIG_USB_MODEM
#ifdef RTCONFIG_USB_BECEEM
	system("killall wimaxd");
	system("killall -SIGUSR1 wimaxd");
	sleep(1);
#endif
	remove_usb_modem_modules();
#endif

// #ifdef RTCONFIG_USB_PRINTER
#ifdef TCSUPPORT_USB_PRINTER_SERVER
	stop_lpd();
	stop_u2ec();
	modprobe_r(USBPRINTER_MOD);
#endif

#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
#if defined(RTCONFIG_APP_PREINSTALLED) && defined(RTCONFIG_CLOUDSYNC)
	if(pids("inotify") || pids("asuswebstorage") || pids("webdav_client") || pids("dropbox_client") || pids("ftpclient") || pids("sambaclient") || pids("usbclient")){
		_dprintf("%s: stop_cloudsync.\n", __FUNCTION__);
		stop_cloudsync();
	}
#endif

	stop_app();
#endif
#ifdef RTCONFIG_MEDIA_SERVER
	force_stop_dms();
#endif

#ifdef RTCONFIG_WEBDAV
	stop_webdav();
#endif

	stop_nas_services(0);

#if defined(RTCONFIG_SAMBASRV) || defined(RTCONFIG_FTP)
	// only stop storage services if disabled
	if (disabled) {//if (disabled || !nvram_get_int("usb_storage")) {
		// Unmount all partitions
		remove_storage_main(0);

		// Stop storage services
		modprobe_r("ext2");
		modprobe_r("ext3");
		modprobe_r("jbd");
#ifdef LINUX26
		modprobe_r("mbcache");
#endif
		// modprobe_r("vfat");
		// modprobe_r("fat");
#ifdef TCSUPPORT_USB_NTFS_UFSD
		modprobe_r("ufsd");
#endif
#ifdef TCSUPPORT_USB_NTFS
		modprobe_r("fuse");
#endif
		sleep(1);
#ifdef RTCONFIG_SAMBASRV
		// modprobe_r("nls_cp437");
		// modprobe_r("nls_cp850");
		// modprobe_r("nls_cp852");
		// modprobe_r("nls_cp866");
#ifdef LINUX26
		// modprobe_r("nls_cp932");
		modprobe_r("nls_cp936");
		// modprobe_r("nls_cp949");
		modprobe_r("nls_cp950");
#endif
#endif
		eval("rmmod", USBSTORAGE_MOD);// modprobe_r(USBSTORAGE_MOD);
		modprobe_r(SD_MOD);
		modprobe_r(SG_MOD);
#ifdef LINUX26
		modprobe_r(SCSI_WAIT_MOD);
#endif
		modprobe_r(SCSI_MOD);
	}
#endif

	// if (disabled || nvram_get_int("usb_ohci") != 1) modprobe_r(USBOHCI_MOD);
	// if (disabled || nvram_get_int("usb_uhci") != 1) modprobe_r(USBUHCI_MOD);
	// if (disabled || nvram_get_int("usb_usb2") != 1) modprobe_r(USB20_MOD);

#ifdef LINUX26
	// modprobe_r("leds-usb");
	// modprobe_r("ledtrig-usbdev");
#endif

	// only unload core modules if usb is disabled
	if (disabled) {
		umount("/proc/bus/usb"); // unmount usb device filesystem
		eval("rmmod", USBOHCI_MOD);// modprobe_r(USBOHCI_MOD);
		// modprobe_r(USBUHCI_MOD);
		eval("rmmod", USB20_MOD);// modprobe_r(USB20_MOD);
		modprobe_r(USBCORE_MOD);
	}
}



// #ifdef RTCONFIG_USB_PRINTER
#ifdef TCSUPPORT_USB_PRINTER_SERVER
void start_usblpsrv()
{
	// nvram_set("u2ec_device", "");
	// nvram_set("u2ec_busyip", "");
	// nvram_set("MFP_busy", "0");
	tcapi_set(USB_DATA, "u2ec_device", "");
	tcapi_set(USB_DATA, "u2ec_busyip", "");
	tcapi_set(USB_DATA, "MFP_busy", "0");

	bin_sem_init();
	start_u2ec();
	start_lpd();
}
#endif

#define MOUNT_VAL_FAIL 	0
#define MOUNT_VAL_RONLY	1
#define MOUNT_VAL_RW 	2
#define MOUNT_VAL_EXIST	3

int mount_r(char *mnt_dev, char *mnt_dir, char *type)
{
	struct mntent *mnt;
	int ret;
	char options[140];
	char flagfn[128];
	int dir_made;

	if ((mnt = findmntents(mnt_dev, 0, NULL, 0))) {
		syslog(LOG_INFO, "USB partition at %s already mounted on %s",
			mnt_dev, mnt->mnt_dir);
		return MOUNT_VAL_EXIST;
	}

	options[0] = 0;

	if (type) {
		unsigned long flags = MS_NOATIME | MS_NODEV;

		if (strcmp(type, "swap") == 0 || strcmp(type, "mbr") == 0) {
			/* not a mountable partition */
			flags = 0;
		}
		else if(!strncmp(type, "ext", 3)){
			sprintf(options, "user_xattr");

			// if (nvram_invmatch("usb_ext_opt", ""))
				// sprintf(options + strlen(options), "%s%s", options[0] ? "," : "", nvram_safe_get("usb_ext_opt"));
		}
		else if (strcmp(type, "vfat") == 0) {
			// sprintf(options, "umask=0000,allow_utime=0000");
			sprintf(options, "umask=0000");	//not support allow_utime option, Sam 2012/10/09

			// if (nvram_invmatch("smbd_cset", ""))
				// sprintf(options + strlen(options), ",iocharset=%s%s", 
					// isdigit(nvram_get("smbd_cset")[0]) ? "cp" : "",
						// nvram_get("smbd_cset"));
			// if (nvram_invmatch("smbd_cpage", "")) {
				// char *cp = nvram_safe_get("smbd_cpage");
				// sprintf(options + strlen(options), ",codepage=%s" + (options[0] ? 0 : 1), cp);
				// sprintf(flagfn, "nls_cp%s", cp);
				// TRACE_PT("USB %s(%s) is setting the code page to %s!\n", mnt_dev, type, flagfn);

				// cp = nvram_get("smbd_nlsmod");
				// if ((cp) && (*cp != 0) && (strcmp(cp, flagfn) != 0))
					// modprobe_r(cp);

				// modprobe(flagfn);
				// nvram_set("smbd_nlsmod", flagfn);
			// }
			sprintf(options + strlen(options), ",shortname=winnt" + (options[0] ? 0 : 1));
#ifdef LINUX26
			sprintf(options + strlen(options), ",flush" + (options[0] ? 0 : 1));
#endif
			// if (nvram_invmatch("usb_fat_opt", ""))
				// sprintf(options + strlen(options), "%s%s", options[0] ? "," : "", nvram_safe_get("usb_fat_opt"));

		}
		else if (strncmp(type, "ntfs", 4) == 0) {
			sprintf(options, "umask=0000");

			// if (nvram_invmatch("smbd_cset", ""))
				// sprintf(options + strlen(options), "%siocharset=%s%s", options[0] ? "," : "",
					// isdigit(nvram_get("smbd_cset")[0]) ? "cp" : "",
						// nvram_get("smbd_cset"));

			// if (nvram_invmatch("usb_ntfs_opt", ""))
				// sprintf(options + strlen(options), "%s%s", options[0] ? "," : "", nvram_safe_get("usb_ntfs_opt"));
		}

		if (flags) {
			if ((dir_made = mkdir_if_none(mnt_dir))) {
				/* Create the flag file for remove the directory on dismount. */
				sprintf(flagfn, "%s/.autocreated-dir", mnt_dir);
				f_write(flagfn, NULL, 0, 0, 0);
			}

			ret = mount(mnt_dev, mnt_dir, type, flags, options[0] ? options : "");
			if(ret != 0){
				syslog(LOG_INFO, "USB %s(%s) failed to mount at the first try!"
						, mnt_dev, type);
				TRACE_PT("USB %s(%s) failed to mount at the first try!\n", mnt_dev, type);
			}

// #ifdef RTCONFIG_NTFS

			/* try ntfs-3g in case it's installed */
			if (ret != 0 && strncmp(type, "ntfs", 4) == 0) {
				sprintf(options + strlen(options), ",noatime,nodev" + (options[0] ? 0 : 1));

				// if (nvram_get_int("usb_fs_ntfs")) {
					// if(!nvram_get_int("usb_fs_ntfs3g")){
						// if(nvram_get_int("usb_fs_ntfs_sparse"))
							// ret = eval("mount", "-t", "ufsd", "-o", options, "-o", "force", "-o", "sparse", mnt_dev, mnt_dir);
						// else
#ifdef TCSUPPORT_USB_NTFS_UFSD
							ret = eval("mount", "-t", "ufsd", "-o", options, "-o", "force", mnt_dev, mnt_dir);
#endif
					// }
					// else 
#ifdef TCSUPPORT_USB_NTFS
						ret = eval("ntfs-3g", "-o", options, mnt_dev, mnt_dir);
#endif
				// }
			}


			//if (ret != 0) /* give it another try - guess fs */
				//ret = eval("mount", "-o", "noatime,nodev", mnt_dev, mnt_dir);

			if (ret == 0) {
				syslog(LOG_INFO, "USB %s%s fs at %s mounted on %s",
					type, (flags & MS_RDONLY) ? " (ro)" : "", mnt_dev, mnt_dir);
				return (flags & MS_RDONLY) ? MOUNT_VAL_RONLY : MOUNT_VAL_RW;
			}

			if (dir_made) {
				unlink(flagfn);
				rmdir(mnt_dir);
			}
		}
	}
	return MOUNT_VAL_FAIL;
}


struct mntent *mount_fstab(char *dev_name, char *type, char *label, char *uuid)
{
	struct mntent *mnt = NULL;
#if 0
	if (eval("mount", "-a") == 0)
		mnt = findmntents(dev_name, 0, NULL, 0);
#else
	char spec[PATH_MAX+1];

	if (label && *label) {
		sprintf(spec, "LABEL=%s", label);
		if (eval("mount", spec) == 0)
			mnt = findmntents(dev_name, 0, NULL, 0);
	}

	if (!mnt && uuid && *uuid) {
		sprintf(spec, "UUID=%s", uuid);
		if (eval("mount", spec) == 0)
			mnt = findmntents(dev_name, 0, NULL, 0);
	}

	if (!mnt) {
		if (eval("mount", dev_name) == 0)
			mnt = findmntents(dev_name, 0, NULL, 0);
	}

	if (!mnt) {
		/* Still did not find what we are looking for, try absolute path */
		if (realpath(dev_name, spec)) {
			if (eval("mount", spec) == 0)
				mnt = findmntents(dev_name, 0, NULL, 0);
		}
	}
#endif

	if (mnt)
		syslog(LOG_INFO, "USB %s fs at %s mounted on %s", type, dev_name, mnt->mnt_dir);
	return (mnt);
}


/* Check if the UFD is still connected because the links created in /dev/discs
 * are not removed when the UFD is  unplugged.
 * The file to read is: /proc/scsi/usb-storage-#/#, where # is the host no.
 * We are looking for "Attached: Yes".
 */
static int usb_ufd_connected(int host_no)
{
	char proc_file[128];
#ifndef LINUX26
	char line[256];
#endif
	FILE *fp;

	sprintf(proc_file, "%s/%s-%d/%d", PROC_SCSI_ROOT, USB_STORAGE, host_no, host_no);
	fp = fopen(proc_file, "r");

	if (!fp) {
		/* try the way it's implemented in newer kernels: /proc/scsi/usb-storage/[host] */
		sprintf(proc_file, "%s/%s/%d", PROC_SCSI_ROOT, USB_STORAGE, host_no);
		fp = fopen(proc_file, "r");
	}

	if (fp) {
#ifdef LINUX26
		fclose(fp);
		return 1;
#else
		while (fgets(line, sizeof(line), fp) != NULL) {
			if (strstr(line, "Attached: Yes")) {
				fclose(fp);
				return 1;
			}
		}
		fclose(fp);
#endif
	}

	return 0;
}


#ifndef MNT_DETACH
#define MNT_DETACH	0x00000002      /* from linux/fs.h - just detach from the tree */
#endif
int umount_mountpoint(struct mntent *mnt, uint flags);
int uswap_mountpoint(struct mntent *mnt, uint flags);

/* Unmount this partition from all its mountpoints.  Note that it may
 * actually be mounted several times, either with different names or
 * with "-o bind" flag.
 * If the special flagfile is now revealed, delete it and [attempt to] delete
 * the directory.
 */
int umount_partition(char *dev_name, int host_num, char *dsc_name, char *pt_name, uint flags)
{
	sync();	/* This won't matter if the device is unplugged, though. */

	if (flags & EFH_HUNKNOWN) {
		/* EFH_HUNKNOWN flag is passed if the host was unknown.
		 * Only unmount disconnected drives in this case.
		 */
		if (usb_ufd_connected(host_num))
			return 0;
	}

	/* Find all the active swaps that are on this device and stop them. */
	findmntents(dev_name, 1, uswap_mountpoint, flags);

	/* Find all the mountpoints that are for this device and unmount them. */
	findmntents(dev_name, 0, umount_mountpoint, flags);

#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
	usb_notify();
#endif

	return 0;
}

int uswap_mountpoint(struct mntent *mnt, uint flags)
{
	swapoff(mnt->mnt_fsname);
	return 0;
}

int umount_mountpoint(struct mntent *mnt, uint flags)
{
	int ret = 1, count;
	char flagfn[128];

	sprintf(flagfn, "%s/.autocreated-dir", mnt->mnt_dir);

	/* Run user pre-unmount scripts if any. It might be too late if
	 * the drive has been disconnected, but we'll try it anyway.
 	 */
	// if (nvram_get_int("usb_automount"))
		// run_nvscript("script_usbumount", mnt->mnt_dir, 3);
	/* Run *.autostop scripts located in the root of the partition being unmounted if any. */
	//run_userfile(mnt->mnt_dir, ".autostop", mnt->mnt_dir, 5);
	//run_nvscript("script_autostop", mnt->mnt_dir, 5);
#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
#if defined(RTCONFIG_APP_PREINSTALLED) && defined(RTCONFIG_CLOUDSYNC)
	char word[PATH_MAX], *next_word;
	char cloud_setting[MAXLEN_TCAPI_MSG];
	char *b, *nvp;
	char sync_dir[PATH_MAX];

	// cloud_setting = nvram_safe_get("cloud_sync");
	nvp = cloud_setting;
	if( !tcapi_get(CLOUDSYNC, "cloud_sync", cloud_setting) ) {
	// if(nv){
		while((b = strsep(&nvp, "<")) != NULL){
			count = 0;
			foreach_62(word, b, next_word){
				switch(count){
					case 5: // dir
						memset(sync_dir, 0, PATH_MAX);
						strncpy(sync_dir, word, PATH_MAX);
						break;
				}
				++count;
			}
		}
		// free(nv);
	}
_dprintf("cloudsync: dir=%s.\n", sync_dir);

	char mounted_path[PATH_MAX], *ptr, *other_path;

	ptr = sync_dir+strlen(POOL_MOUNT_ROOT)+1;
_dprintf("cloudsync: ptr=%s.\n", ptr);
	if((other_path = strchr(ptr, '/')) != NULL){
		ptr = other_path;
		++other_path;
	}
	else
		ptr = "";
_dprintf("cloudsync: other_path=%s.\n", other_path);

	memset(mounted_path, 0, PATH_MAX);
	strncpy(mounted_path, sync_dir, (strlen(sync_dir)-strlen(ptr)));
_dprintf("cloudsync: mounted_path=%s.\n", mounted_path);

	if(!strcmp(mounted_path, mnt->mnt_dir)){
		_dprintf("%s: stop_cloudsync.\n", __FUNCTION__);
		stop_cloudsync();
	}
#endif

	// if(nvram_match("apps_mounted_path", mnt->mnt_dir))
	if(tcapi_match(APPS_DATA, "apps_mounted_path", mnt->mnt_dir))
		stop_app();
#endif

#ifdef TCSUPPORT_SWAP_FILE
	stop_swapfile(mnt->mnt_dir);
#endif

	count = 0;
	while ((ret = umount(mnt->mnt_dir)) && (count < 2)) {
		count++;
		/* If we could not unmount the drive on the 1st try,
		 * kill all NAS applications so they are not keeping the device busy -
		 * unless it's an unmount request from the Web GUI.
		 */
		if ((count == 1) && ((flags & EFH_USER) == 0)){
//_dprintf("restart_nas_services(%d): test 1.\n", getpid());
			restart_nas_services(1, 0);
		}
		sleep(1);
	}

	if (ret == 0)
		syslog(LOG_INFO, "USB partition unmounted from %s", mnt->mnt_dir);

	if (ret && ((flags & EFH_SHUTDN) != 0)) {
		/* If system is stopping (not restarting), and we couldn't unmount the
		 * partition, try to remount it as read-only. Ignore the return code -
		 * we can still try to do a lazy unmount.
		 */
		eval("mount", "-o", "remount,ro", mnt->mnt_dir);
	}

	if (ret && ((flags & EFH_USER) == 0)) {
		/* Make one more try to do a lazy unmount unless it's an unmount
		 * request from the Web GUI.
		 * MNT_DETACH will expose the underlying mountpoint directory to all
		 * except whatever has cd'ed to the mountpoint (thereby making it busy).
		 * So the unmount can't actually fail. It disappears from the ken of
		 * everyone else immediately, and from the ken of whomever is keeping it
		 * busy when they move away from it. And then it disappears for real.
		 */
		ret = umount2(mnt->mnt_dir, MNT_DETACH);
		syslog(LOG_INFO, "USB partition busy - will unmount ASAP from %s", mnt->mnt_dir);
	}

	if (ret == 0) {
		if ((unlink(flagfn) == 0)) {
			// Only delete the directory if it was auto-created
			rmdir(mnt->mnt_dir);
		}
	}

#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
	// if(nvram_match("apps_mounted_path", mnt->mnt_dir)){
	if(tcapi_match(APPS_DATA, "apps_mounted_path", mnt->mnt_dir)) {
		// nvram_set("apps_dev", "");
		// nvram_set("apps_mounted_path", "");
		tcapi_set(APPS_DATA, "apps_dev", "");
		tcapi_set(APPS_DATA, "apps_mounted_path", "");
	}
#endif

	tcapi_save();	//Andy Chiu, 2015/03/19
	return (ret == 0);
}

//Andy Chiu, 2015/04/24 add.
static int diskmon_status(int status)
{
	static int run_status = DISKMON_IDLE;
	int old_status = run_status;
	char *message;
	char buf[8];

	switch (status) {
	case DISKMON_IDLE:
		message = "be idle";
		break;
	case DISKMON_START:
		message = "start...";
		break;
	case DISKMON_UMOUNT:
		message = "unmount partition";
		break;
	case DISKMON_SCAN:
		message = "scan partition";
		break;
	case DISKMON_REMOUNT:
		message = "re-mount partition";
		break;
	case DISKMON_FINISH:
		message = "done";
		break;
	case DISKMON_FORCE_STOP:
		message = "forcely stop";
		break;
	default:
		/* Just return previous status */
		return old_status;
	}

	/* Set new status */
	run_status = status;
	sprintf(buf, "%d", status);
	tcapi_set("USB_Entry", "diskmon_status", buf);
	//nvram_set_int("diskmon_status", status);
	logmessage("disk monitor", message);
	return old_status;
}


/* Mount this partition on this disc.
 * If the device is already mounted on any mountpoint, don't mount it again.
 * If this is a swap partition, try swapon -a.
 * If this is a regular partition, try mount -a.
 *
 * Before we mount any partitions:
 *	If the type is swap and /etc/fstab exists, do "swapon -a"
 *	If /etc/fstab exists, try mounting using fstab.
 *  We delay invoking mount because mount will probe all the partitions
 *	to read the labels, and we don't want it to do that early on.
 *  We don't invoke swapon until we actually find a swap partition.
 *
 * If the mount succeeds, execute the *.asusrouter scripts in the top
 * directory of the newly mounted partition.
 * Returns NZ for success, 0 if we did not mount anything.
 */
int mount_partition(char *dev_name, int host_num, char *dsc_name, char *pt_name, uint flags)
{
	char the_label[128], mountpoint[128], uuid[40];
	int ret;
	char *type, *ptr;
	// static char *swp_argv[] = { "swapon", "-a", NULL };
	struct mntent *mnt;
	char command[PATH_MAX];

	if ((type = detect_fs_type(dev_name)) == NULL)
		return 0;

#ifdef VOLUME_NAME
	find_label_or_uuid(dev_name, the_label, uuid);
#endif

	if(!is_valid_hostname(the_label))
		memset(the_label, 0, 128);

	if (f_exists("/etc/fstab")) {
		if (strcmp(type, "swap") == 0) {
			// _eval(swp_argv, NULL, 0, NULL);
			swapon(dev_name, 0);	//Use swap partition whether in fstab or not.
			return 0;
		}
		if (mount_r(dev_name, NULL, NULL) == MOUNT_VAL_EXIST)
			return 0;
		if ((mnt = mount_fstab(dev_name, type, the_label, uuid))) {
			strcpy(mountpoint, mnt->mnt_dir);
			ret = MOUNT_VAL_RW;
			goto done;
		}
	}

	if (*the_label != 0) {
		for (ptr = the_label; *ptr; ptr++) {
			if (!isalnum(*ptr) && !strchr("+-&.@", *ptr))
				*ptr = '_';
		}
		sprintf(mountpoint, "%s/%s", POOL_MOUNT_ROOT, the_label);

		int the_same_name = 0;
		while(check_if_file_exist(mountpoint) || check_if_dir_exist(mountpoint)){
			++the_same_name;
			sprintf(mountpoint, "%s/%s(%d)", POOL_MOUNT_ROOT, the_label, the_same_name);
		}

		if ((ret = mount_r(dev_name, mountpoint, type)))
			goto done;
	}

	/* Can't mount to /mnt/LABEL, so try mounting to /mnt/discDN_PN */
	sprintf(mountpoint, "%s/%s", POOL_MOUNT_ROOT, pt_name);
	ret = mount_r(dev_name, mountpoint, type);
	cprintf("ret=%d, dev_name=[%s], ptr=[%s]\n", ret, dev_name, dev_name+5);
done:
	if (ret == MOUNT_VAL_RONLY || ret == MOUNT_VAL_RW)
	{
		chmod(mountpoint, 0777);

		char usb_node[32], port_path[8];
		char prefix[] = "usb_pathXXXXXXXXXXXXXXXXX_", tmp[100];
	#ifdef RTCONFIG_USB_MODEM
		unsigned int vid, pid;
	#endif

		ptr = dev_name+5;

		// Get USB node.
		if(get_usb_node_by_device(ptr, usb_node, 32) != NULL){
			if(get_path_by_node(usb_node, port_path, 8) != NULL){
				snprintf(prefix, sizeof(prefix), "usb_path%s", port_path);
				// for ATE.
				if(strlen(nvram_safe_get(strcat_r(prefix, "_fs_path0", tmp))) <= 0)
					nvram_set(tmp, ptr);
			#ifdef RTCONFIG_USB_MODEM
				vid = strtoul(nvram_safe_get(strcat_r(prefix, "_vid", tmp)), NULL, 16);
				pid = strtoul(nvram_safe_get(strcat_r(prefix, "_pid", tmp)), NULL, 16);

				if(is_create_file_dongle(vid, pid)){
					if(strcmp(nvram_safe_get("stop_sg_remove"), "1")){
						memset(command, 0, PATH_MAX);
						sprintf(command, "touch %s/wcdma.cfg", mountpoint);
						system(command);
					}

					return 0; // skip to restart_nasapps.
				}
			#endif /* RTCONFIG_USB_MODEM */
			}
		}

#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
		// if(!strcmp(nvram_safe_get("apps_mounted_path"), "")){
		if(tcapi_match(APPS_DATA, "apps_mounted_path", "")){
			char apps_folder[32], buff1[64], buff2[64];

			tcapi_get("Apps_Entry", "apps_install_folder", apps_folder);

			memset(command, 0, PATH_MAX);
			sprintf(command, "app_check_folder.sh %s", mountpoint);
			system(command);

			memset(buff1, 0, 64);
			sprintf(buff1, "%s/%s/.asusrouter", mountpoint, apps_folder);
			memset(buff2, 0, 64);
			sprintf(buff2, "%s/%s/.asusrouter.disabled", mountpoint, apps_folder);

			if(check_if_file_exist(buff1) && !check_if_file_exist(buff2)){
				// fsck the partition.
				if(!strcmp(type, "ext2") || !strcmp(type, "ext3")
						|| !strcmp(type, "vfat") || !strcmp(type, "msdos")
						|| !strcmp(type, "ntfs") || !strcmp(type, "ufsd")
						){
					findmntents(dev_name, 0, umount_mountpoint, EFH_HP_REMOVE);
					memset(command, 0, PATH_MAX);
					sprintf(command, "app_fsck.sh %s %s", type, dev_name);
					system(command);
					mount_r(dev_name, mountpoint, type);
				}

				system("rm -rf /tmp/opt");

				memset(command, 0, PATH_MAX);
				sprintf(command, "ln -sf %s/%s /tmp/opt", mountpoint, apps_folder);
				system(command);

				memset(buff1, 0, 64);
				sprintf(buff1, "APPS_DEV=%s", dev_name+5);
				putenv(buff1);
				memset(buff2, 0, 64);
				sprintf(buff2, "APPS_MOUNTED_PATH=%s", mountpoint);
				putenv(buff2);
				/* Run user *.asusrouter and post-mount scripts if any. */
				memset(command, 0, PATH_MAX);
				//sprintf(command, "%s/%s", mountpoint, apps_folder);
				// run_userfile(command, ".asusrouter", NULL, 3);
				sprintf(command, "%s/%s/.asusrouter", mountpoint, apps_folder);
				system(command);
				unsetenv("APPS_DEV");
				unsetenv("APPS_MOUNTED_PATH");
				tcapi_save();	//Andy Chiu, 2015/03/18
			}
		}

		usb_notify();
#endif

#ifdef TCSUPPORT_SWAP_FILE
		start_swapfile(mountpoint);
#endif

#ifdef TCSUPPORT_DSL_LINE_DIAGNOSTIC
		if(ret == MOUNT_VAL_RW) {
			int DiagMdoe = tcapi_get_int("DslDiag_Entry", "dslx_diag_enable");
			tcapi_set("DslDiag_Entry", "dslx_diag_log_path", mountpoint);
			/* DIAG enable || (DIAG disable && DSL_DIAG_STATE_SENDMAIL_FAIL_SMTP) */
			if(DiagMdoe ||
				(tcapi_match("DslDiag_Entry", "dslx_diag_enable", "0") && tcapi_match("DslDiag_Entry", "dslx_diag_state", "4"))) {
				_dprintf("%s: trigger DSL line / Wi-Fi diagnostic.\n", __FUNCTION__);
				tcapi_commit("DslDiag_Entry");
			}
		}
		else {
			logmessage("DSL / Wi-Fi Diagnostic", "USB start failed");
		}
#endif /* TCSUPPORT_DSL_LINE_DIAGNOSTIC */

		// check the permission files.
		if(ret == MOUNT_VAL_RW) {
			test_of_var_files(mountpoint);
		}

		// if (nvram_get_int("usb_automount"))
			// run_nvscript("script_usbmount", mountpoint, 3);

#if defined(RTCONFIG_APP_PREINSTALLED) && defined(RTCONFIG_CLOUDSYNC)
		char word[PATH_MAX], *next_word;
		char cloud_setting[MAXLEN_TCAPI_MSG] = {0};
		char *b, *nvp;
		int type = 0, rule = 0, enable = 0;
		char username[64], password[64], url[PATH_MAX], sync_dir[PATH_MAX];
		int count;
		char cloud_token[PATH_MAX];

		// cloud_setting = nvram_safe_get("cloud_sync");
		tcapi_get(CLOUDSYNC, "cloud_sync", cloud_setting);

		// if(!nvram_get_int("enable_cloudsync") || strlen(cloud_setting) <= 0)
		if(!tcapi_get_int(CLOUDSYNC, "enable_cloudsync") || strlen(cloud_setting) <= 0)
			return (ret == MOUNT_VAL_RONLY || ret == MOUNT_VAL_RW);

		if(pids("inotify") || pids("asuswebstorage") || pids("webdav_client") || pids("dropbox_client") || pids("ftpclient") || pids("sambaclient") || pids("usbclient"))
			return (ret == MOUNT_VAL_RONLY || ret == MOUNT_VAL_RW);

		// nv = nvp = strdup(nvram_safe_get("cloud_sync"));
		nvp = cloud_setting;
		// if(nv){
			while((b = strsep(&nvp, "<")) != NULL){
				count = 0;
				foreach_62(word, b, next_word){
					switch(count){
						case 0: // type
							type = atoi(word);
							break;
					}
					++count;
				}

				if(type == 1 || type == 2 || type == 3 || type == 4 || type == 5 ){
						start_cloudsync();
				}
				else{
					count = 0;
					foreach_62(word, cloud_setting, next_word){
						switch(count){
							case 0: // type
								type = atoi(word);
								break;
							case 1: // username
								memset(username, 0, 64);
								strncpy(username, word, 64);
								break;
							case 2: // password
								memset(password, 0, 64);
								strncpy(password, word, 64);
								break;
							case 3: // url
								memset(url, 0, PATH_MAX);
								strncpy(url, word, PATH_MAX);
								break;
							case 4: // rule
								rule = atoi(word);
								break;
							case 5: // dir
								memset(sync_dir, 0, PATH_MAX);
								strncpy(sync_dir, word, PATH_MAX);
								break;
							case 6: // enable
								enable = atoi(word);
								break;
						}

						++count;
					}
_dprintf("cloudsync: enable=%d, type=%d, user=%s, dir=%s.\n", enable, type, username, sync_dir);

					if(!enable)
						return (ret == MOUNT_VAL_RONLY || ret == MOUNT_VAL_RW);
logmessage("mount_partition", "%d", __LINE__);
					memset(cloud_token, 0, PATH_MAX);
					sprintf(cloud_token, "%s/.__cloudsync_%d_%s.txt", mountpoint, type, username);
_dprintf("cloudsync: cloud_token=%s.\n", cloud_token);

					if(check_if_file_exist(cloud_token)){
						char mounted_path[PATH_MAX], *other_path;
						char true_cloud_setting[PATH_MAX];

						ptr = sync_dir+strlen(POOL_MOUNT_ROOT)+1;
						if((other_path = strchr(ptr, '/')) != NULL){
							ptr = other_path;
							++other_path;
						}
						else
							ptr = "";
_dprintf("cloudsync: ptr=%s.\n", ptr);
_dprintf("cloudsync: other_path=%s.\n", other_path);

						memset(mounted_path, 0, PATH_MAX);
						strncpy(mounted_path, sync_dir, (strlen(sync_dir)-strlen(ptr)));
_dprintf("cloudsync:   mountpoint=%s.\n", mountpoint);
_dprintf("cloudsync: mounted_path=%s.\n", mounted_path);

						if(strcmp(mounted_path, mountpoint)){
							memset(true_cloud_setting, 0, PATH_MAX);
							sprintf(true_cloud_setting, "%d>%s>%s>%s>%d>%s%s%s>%d", type, username, password, url, rule, mountpoint, (other_path != NULL)?"/":"", (other_path != NULL)?other_path:"", enable);
_dprintf("cloudsync: true_cloud_setting=%s.\n", true_cloud_setting);
_dprintf("cloudsync: set nvram....\n");
							// nvram_set("cloud_sync", true_cloud_setting);
							tcapi_set(CLOUDSYNC, "cloud_sync", true_cloud_setting);
_dprintf("cloudsync: wait a second...\n");
							sleep(1); // wait the nvram be ready.
_dprintf("cloudsync: finished.\n");
						}

_dprintf("%s: start_cloudsync.\n", __FUNCTION__);
						start_cloudsync();
					}
				}
			}
			// free(nv);
		// }
#endif
	}
	return (ret == MOUNT_VAL_RONLY || ret == MOUNT_VAL_RW);
}

/* Mount or unmount all partitions on this controller.
 * Parameter: action_add:
 * 0  = unmount
 * >0 = mount only if automount config option is enabled.
 * <0 = mount regardless of config option.
 */
void hotplug_usb_storage_device(int host_no, int action_add, uint flags)
{
	// if (!nvram_get_int("usb_enable"))
		// return;

	_dprintf("%s: host %d action: %d\n", __FUNCTION__, host_no, action_add);

	if (action_add) {
		// if (nvram_get_int("usb_storage") && (nvram_get_int("usb_automount") || action_add < 0)) {
			/* Do not probe the device here. It's either initiated by user,
			 * or hotplug_usb() already did.
			 */
			if (exec_for_host(host_no, 0x00, flags, mount_partition)) {
//_dprintf("restart_nas_services(%d): test 2.\n", getpid());
				restart_nas_services(1, 1); // restart all NAS applications
			}
		// }
	}
	else {
		// if (nvram_get_int("usb_storage") || ((flags & EFH_USER) == 0)) {
			/* When unplugged, unmount the device even if
			 * usb storage is disabled in the GUI.
			 */
			exec_for_host(host_no, (flags & EFH_USER) ? 0x00 : 0x02, flags, umount_partition);
			/* Restart NAS applications (they could be killed by umount_mountpoint),
			 * or just re-read the configuration.
			 */
//_dprintf("restart_nas_services(%d): test 3.\n", getpid());
			restart_nas_services(1, 1);
		// }
	}
}


/* This gets called at reboot or upgrade.  The system is stopping. */
void remove_storage_main(int shutdn)
{
	if (shutdn){
//_dprintf("restart_nas_services(%d): test 4.\n", getpid());
		restart_nas_services(1, 0);
	}
	/* Unmount all partitions */
	exec_for_host(-1, 0x02, shutdn ? EFH_SHUTDN : 0, umount_partition);
}


/*******
 * All the complex locking & checking code was removed when the kernel USB-storage
 * bugs were fixed.
 * The crash bug was with overlapped I/O to different USB drives, not specifically
 * with mount processing.
 *
 * And for USB devices that are slow to come up.  The kernel now waits until the
 * USB drive has settled, and it correctly reads the partition table before calling
 * the hotplug agent.
 *
 * The kernel patch was cleaning up data structures on an unplug.  It
 * needs to wait until the disk is unmounted.  We have 20 seconds to do
 * the unmounts.
 *******/


/* Plugging or removing usb device
 *
 * On an occurrance, multiple hotplug events may be fired off.
 * For example, if a hub is plugged or unplugged, an event
 * will be generated for everything downstream of it, plus one for
 * the hub itself.  These are fired off simultaneously, not serially.
 * This means that many many hotplug processes will be running at
 * the same time.
 *
 * The hotplug event generated by the kernel gives us several pieces
 * of information:
 * PRODUCT is vendorid/productid/rev#.
 * DEVICE is /proc/bus/usb/bus#/dev#
 * ACTION is add or remove
 * SCSI_HOST is the host (controller) number (this relies on the custom kernel patch)
 *
 * Note that when we get a hotplug add event, the USB susbsystem may
 * or may not have yet tried to read the partition table of the
 * device.  For a new controller that has never been seen before,
 * generally yes.  For a re-plug of a controller that has been seen
 * before, generally no.
 *
 * On a remove, the partition info has not yet been expunged.  The
 * partitions show up as /dev/discs/disc#/part#, and /proc/partitions.
 * It appears that doing a "stat" for a non-existant partition will
 * causes the kernel to re-validate the device and update the
 * partition table info.  However, it won't re-validate if the disc is
 * mounted--you'll get a "Device busy for revalidation (usage=%d)" in
 * syslog.
 *
 * The $INTERFACE is "class/subclass/protocol"
 * Some interesting classes:
 *	8 = mass storage
 *	7 = printer
 *	3 = HID.   3/1/2 = mouse.
 *	6 = still image (6/1/1 = Digital camera Camera)
 *	9 = Hub
 *	255 = scanner (255/255/255)
 *
 * Observed:
 *	Hub seems to have no INTERFACE (null), and TYPE of "9/0/0"
 *	Flash disk seems to have INTERFACE of "8/6/80", and TYPE of "0/0/0"
 *
 * When a hub is unplugged, a hotplug event is generated for it and everything
 * downstream from it.  You cannot depend on getting these events in any
 * particular order, since there will be many hotplug programs all fired off
 * at almost the same time.
 * On a remove, don't try to access the downstream devices right away, give the
 * kernel time to finish cleaning up all the data structures, which will be
 * in the process of being torn down.
 *
 * On the initial plugin, the first time the kernel usb-storage subsystem sees
 * the host (identified by GUID), it automatically reads the partition table.
 * On subsequent plugins, it does not.
 *
 * Special values for Web Administration to unmount or remount
 * all partitions of the host:
 *	INTERFACE=TOMATO/...
 *	ACTION=add/remove
 *	SCSI_HOST=<host_no>
 * If host_no is negative, we unmount all partions of *all* hosts.
 */
void hotplug_usb(void)
{
	int add;
	int host = -1;
	char *interface = getenv("INTERFACE");
	char *action = getenv("ACTION");
	char *product = getenv("PRODUCT");
#ifdef LINUX26
	char *device = getenv("DEVICENAME");
	int is_block = strcmp(getenv("SUBSYSTEM") ? : "", "block") == 0;
#else
	char *device = getenv("DEVICE");
#endif
	char *scsi_host = getenv("SCSI_HOST");
	char *usbport = getenv("USBPORT");

	_dprintf("%s hotplug INTERFACE=%s ACTION=%s USBPORT=%s HOST=%s DEVICE=%s\n",
		getenv("SUBSYSTEM") ? : "USB", interface, action, usbport, scsi_host, device);

	// if (!nvram_get_int("usb_enable")) return;
#ifdef LINUX26
	if (!action || ((!interface || !product) && !is_block))
#else
	if (!interface || !action || !product)	/* Hubs bail out here. */
#endif
		return;

	if (scsi_host)
		host = atoi(scsi_host);

	// if (!wait_action_idle(10)) return;

	add = (strcmp(action, "add") == 0);
	if (add && (strncmp(interface ? : "", "TOMATO/", 7) != 0)) {
#ifdef LINUX26
		if (!is_block && device)
#endif
		syslog(LOG_DEBUG, "Attached USB device %s [INTERFACE=%s PRODUCT=%s]",
			device, interface, product);


#ifndef LINUX26
		/* To allow automount to be blocked on startup.
		 * In kernel 2.6 we still need to serialize mount/umount calls -
		 * so the lock is down below in the "block" hotplug processing.
		 */
		file_unlock(file_lock("usb"));
#endif
	}

	if (strncmp(interface ? : "", "TOMATO/", 7) == 0) {	/* web admin */
		if (scsi_host == NULL)
			host = atoi(product);	// for backward compatibility
		/* If host is negative, unmount all partitions of *all* hosts.
		 * If host == -1, execute "soft" unmount (do not kill NAS apps, no "lazy" umount).
		 * If host == -2, run "hard" unmount, as if the drive is unplugged.
		 * This feature can be used in custom scripts as following:
		 *
		 * # INTERFACE=TOMATO/1 ACTION=remove PRODUCT=-1 SCSI_HOST=-1 hotplug usb
		 *
		 * PRODUCT is required to pass the env variables verification.
		 */
		/* Unmount or remount all partitions of the host. */
		hotplug_usb_storage_device(host < 0 ? -1 : host, add ? -1 : 0,
			host == -2 ? 0 : EFH_USER);
	}
#ifdef LINUX26
	else if (is_block && strcmp(getenv("MAJOR") ? : "", "8") == 0
#if !defined(TCSUPPORT_2_6_36_KERNEL)	//TC toolchain is used to build both kernel 2.6.22 and 2.6.36. Change the compile flag
		&& strcmp(getenv("PHYSDEVBUS") ? : "", "scsi") == 0
#endif

		)
	{
		/* scsi partition */
		char devname[64];
		int lock;

		sprintf(devname, "/dev/%s", device);
		lock = file_lock("usb");
		if (add) {
			// if (nvram_get_int("usb_storage") && nvram_get_int("usb_automount")) {
				int minor = atoi(getenv("MINOR") ? : "0");	
				if ((minor % 16) == 0 && !is_no_partition(device)) {
					/* This is a disc, and not a "no-partition" device,
					 * like APPLE iPOD shuffle. We can't mount it.
					 */
					return;
				}
				TRACE_PT(" mount to dev: %s\n", devname);
				if (mount_partition(devname, host, NULL, device, EFH_HP_ADD)) {
//_dprintf("restart_nas_services(%d): test 5.\n", getpid());
					restart_nas_services(1, 1); // restart all NAS applications
					// notify_rc_after_wait("restart_nasapps");
				}
				TRACE_PT(" end of mount\n");
			// }
		}
		else {
			/* When unplugged, unmount the device even if usb storage is disabled in the GUI */
			umount_partition(devname, host, NULL, device, EFH_HP_REMOVE);
			/* Restart NAS applications (they could be killed by umount_mountpoint),
			 * or just re-read the configuration.
			 */
//_dprintf("restart_nas_services(%d): test 6.\n", getpid());
			restart_nas_services(1, 1);
			// notify_rc_after_wait("restart_nasapps");
		}
		file_unlock(lock);
	}
#endif
	else if (strncmp(interface ? : "", "8/", 2) == 0) {	/* usb storage */
		// run_nvscript("script_usbhotplug", NULL, 2);
#ifndef LINUX26
		hotplug_usb_storage_device(host, add, (add ? EFH_HP_ADD : EFH_HP_REMOVE) | (host < 0 ? EFH_HUNKNOWN : 0));
#endif
	}
	else {	/* It's some other type of USB device, not storage. */
#ifdef LINUX26
		if (is_block) return;
#endif
		/* Do nothing.  The user's hotplug script must do it all. */
		// run_nvscript("script_usbhotplug", NULL, 2);
	}
}



// -----------------------------------------------------------------------------

// !!TB - FTP Server

#ifdef RTCONFIG_FTP
/* VSFTPD code mostly stolen from Oleg's ASUS Custom Firmware GPL sources */

void write_ftpd_conf()
{
	FILE *fp;
	char tmp[16];

	/* write /etc/vsftpd.conf */
	fp=fopen("/etc/vsftpd.conf", "w");
	if (fp==NULL) return;

	// if (nvram_match("st_ftp_mode", "2"))
	if (tcapi_match(SAMBA_DATA, "st_ftp_mode", "2"))
		fprintf(fp, "anonymous_enable=NO\n");
	else{
		fprintf(fp, "anonymous_enable=YES\n");
		fprintf(fp, "anon_upload_enable=YES\n");
		fprintf(fp, "anon_mkdir_write_enable=YES\n");
		fprintf(fp, "anon_other_write_enable=YES\n");
	}

	fprintf(fp, "nopriv_user=root\n");
	fprintf(fp, "write_enable=YES\n");
	fprintf(fp, "local_enable=YES\n");
	fprintf(fp, "chroot_local_user=YES\n");
	fprintf(fp, "local_umask=000\n");
	fprintf(fp, "dirmessage_enable=NO\n");
	fprintf(fp, "xferlog_enable=NO\n");
	fprintf(fp, "syslog_enable=NO\n");
	fprintf(fp, "connect_from_port_20=YES\n");
	fprintf(fp, "use_sendfile=NO\n");
//	fprintf(fp, "listen=YES\n");
	fprintf(fp, "listen%s=YES\n",
// #ifdef RTCONFIG_IPV6
	// ipv6_enabled() ? "_ipv6" : "");
#ifdef TCSUPPORT_IPV6
	"_ipv6");
#else
	"");
#endif
	fprintf(fp, "pasv_enable=YES\n");
	fprintf(fp, "ssl_enable=NO\n");
	fprintf(fp, "tcp_wrappers=NO\n");
	// strcpy(maxuser, nvram_safe_get("st_max_user"));
	// if ((atoi(maxuser)) > 0)
		// fprintf(fp, "max_clients=%s\n", maxuser);
	tcapi_get(SAMBA_DATA, "st_max_user", tmp);
	if ((atoi(tmp)) > 0)
		fprintf(fp, "max_clients=%s\n", tmp);
	else
		fprintf(fp, "max_clients=%s\n", "10");
	fprintf(fp, "ftp_username=anonymous\n");
	fprintf(fp, "ftpd_banner=Welcome to ASUS %s FTP service.\n", get_productid());
	fprintf(fp, "hide_file={.router_temp*}\n");

	// update codepage
	// modprobe_r("nls_cp936");
	// modprobe_r("nls_cp950");

	tcapi_get(SAMBA_DATA, "ftp_lang", tmp);
	if (strcmp(tmp, "EN"))
	// if (strcmp(nvram_safe_get("ftp_lang"), "EN") != 0)
	{
		fprintf(fp, "enable_iconv=YES\n");
		// if (nvram_match("ftp_lang", "TW")) {
		if (!strcmp(tmp, "TW")) {
			fprintf(fp, "remote_charset=cp950\n");		
			modprobe("nls_cp950");	
		}
		// else if (nvram_match("ftp_lang", "CN")) {
		else if (!strcmp(tmp, "CN")) {
			fprintf(fp, "remote_charset=cp936\n");
			modprobe("nls_cp936");
		}
	}

	// if(!strcmp(nvram_safe_get("enable_ftp_log"), "1")){
		// fprintf(fp, "xferlog_enable=YES\n");
		// fprintf(fp, "xferlog_file=/var/log/vsftpd.log\n");
	// }

	fclose(fp);
}

/*
 * st_ftp_modex: 0:no-ftp, 1:anonymous, 2:account 
 */

void
start_ftpd(void)
{
	pid_t pid;
#if defined(TCSUPPORT_2_6_36_KERNEL)
	char *vsftpd_argv[] = { "taskset", "0xa", "/userfs/bin/vsftpd", "/etc/vsftpd.conf", NULL };
#else
	char *vsftpd_argv[] = { "/userfs/bin/vsftpd", "/etc/vsftpd.conf", NULL };
#endif

	// if(getpid()!=1) {
		// notify_rc_after_wait("start_ftpd");
		// return;
	// }

	// if (nvram_match("enable_ftp", "0")) return;
	if (tcapi_match(SAMBA_DATA, "enable_ftp", "No")) return;

	write_ftpd_conf();

	killall("vsftpd", SIGHUP);
	
	if (!pids("vsftpd"))
		_eval(vsftpd_argv, NULL, 0, &pid);

	if (pids("vsftpd"))
		logmessage("FTP server", "daemon is started");
}

void stop_ftpd(void)
{
	// if (getpid() != 1) {
		// notify_rc_after_wait("stop_ftpd");
		// return;
	// }

	killall_tk("vsftpd");
	unlink("/tmp/vsftpd.conf");
	logmessage("FTP Server", "daemon is stoped");
}
#endif	// RTCONFIG_FTP

// -----------------------------------------------------------------------------

// !!TB - Samba

#ifdef RTCONFIG_SAMBASRV
void create_custom_passwd()
{
	FILE *fp;
	int result, n=0, i;
	int acc_num;
	char **account_list;

	/* write /etc/passwd.custom */
	fp = fopen("/etc/passwd.custom", "w+");
	result = get_account_list(&acc_num, &account_list);
	if (result < 0) {
		usb_dbg("Can't get the account list.\n");
		free_2_dimension_list(&acc_num, &account_list);
		return;
	}
	for (i=0, n=500; i<acc_num; i++, n++)
	{
		fprintf(fp, "%s:x:%d:%d:::\n", account_list[i], n, n);
	}
	fclose(fp);

	/* write /etc/group.custom  */
	// fp = fopen("/etc/group.custom", "w+");
	// for (i=0, n=500; i<acc_num; i++, n++)
	// {
		// fprintf(fp, "%s:x:%d:\n", account_list[i], n);
	// }
	// fclose(fp);
	free_2_dimension_list(&acc_num, &account_list);
}

static void kill_samba(int sig)
{
	if (sig == SIGTERM) {
		killall_tk("smbd");
		killall_tk("nmbd");
	}
	else {
		killall("smbd", sig);
		killall("nmbd", sig);
	}
}

#if 0 //#ifdef LINUX26
void enable_gro()
{
	char *argv[3] = {"echo", "", NULL};
	char lan_ifname[32], *lan_ifnames, *next;
	char path[64] = {0};

	/* enabled gso on vlan interface */
	lan_ifnames = nvram_safe_get("lan_ifnames");
	foreach(lan_ifname, lan_ifnames, next) {
		if (!strncmp(lan_ifname, "vlan", 4)) {
			sprintf(path, ">>/proc/net/vlan/%s", lan_ifname);
			argv[1] = "-gro 2";
			_eval(argv, path, 0, NULL);
		}
	}
}
#endif

int suit_double_quote(const char *output, const char *input, int outsize){
	char *src = (char *)input;
	char *dst = (char *)output;
	char *end = (char *)output + outsize - 1;

	if(src == NULL || dst == NULL || outsize <= 0)
		return 0;

	for(; *src && dst < end; ++src){
		if(*src =='"'){
			if(dst+2 > end)
				break;

			*dst++ = '\\';
			*dst++ = *src;
		}
		else
			*dst++ = *src;
	}

	if(dst <= end)
		*dst = '\0';

	return dst-output;
}

void
start_samba(void)
{
	int acc_num, i;
	char cmd[256];
	char *nv, *nvp, *b;
	char *tmp_ascii_user, *tmp_ascii_passwd;
	char tmp[MAXLEN_TCAPI_MSG] = {0};

	// if (getpid() != 1) {
		// notify_rc_after_wait("start_samba");
		// return;
	// }

	// if (nvram_match("enable_samba", "0")) return;
	if(!tcapi_match(SAMBA_DATA, "Active", "Yes")) return;

#ifdef RTCONFIG_GROCTRL
	enable_gro();
#endif
	
	mkdir_if_none("/var/run/samba");
	mkdir_if_none("/etc/samba");
	
	unlink("/etc/smb.conf");
	unlink("/etc/smbpasswd");

	/* write samba configure file*/
	system("/userfs/bin/write_smb_conf");

	/* write smbpasswd  */
	// system("smbpasswd nobody \"\"");
	system("/userfs/bin/smbpasswd -a nobody \"\"");

	// acc_num = atoi(nvram_safe_get("acc_num"));
	acc_num = tcapi_get_int(SAMBA_DATA, "acc_num");
	if(acc_num < 0)
		acc_num = 0;

	// nv = nvp = strdup(nvram_safe_get("acc_list"));
	tcapi_get(SAMBA_DATA, "acc_list", tmp);
	nv = nvp = tmp;
	i = 0;
	if(nv && strlen(nv) > 0){
		while((b = strsep(&nvp, "<")) != NULL){
			if(vstrsep(b, ">", &tmp_ascii_user, &tmp_ascii_passwd) != 2)
				continue;

			char char_user[64], char_passwd[64], suit_user[64], suit_passwd[64];

			memset(char_user, 0, 64);
			ascii_to_char_safe(char_user, tmp_ascii_user, 64);
			memset(suit_user, 0, 64);
			suit_double_quote(suit_user, char_user, 64);
			memset(char_passwd, 0, 64);
			ascii_to_char_safe(char_passwd, tmp_ascii_passwd, 64);
			memset(suit_passwd, 0, 64);
			suit_double_quote(suit_passwd, char_passwd, 64);

			sprintf(cmd, "smbpasswd \"%s\" \"%s\"", suit_user, suit_passwd);
//_dprintf("%s: cmd=%s.\n", __FUNCTION__, cmd);
			system(cmd);

			if(++i >= acc_num)
				break;
		}
	}
	// if(nv)
		// free(nv);
	xstart("nmbd", "-D", "-s", "/etc/smb.conf");
#if defined(TCSUPPORT_2_6_36_KERNEL)
	xstart("taskset", "0xa","smbd", "-D", "-s", "/etc/smb.conf");
#else
	xstart("smbd", "-D", "-s", "/etc/smb.conf");
#endif

	logmessage("Samba Server", "daemon is started");

	return;
}

void stop_samba(void)
{
	// if (getpid() != 1) {
		// notify_rc_after_wait("stop_samba");
		// return;
	// }

	kill_samba(SIGTERM);
	/* clean up */
	unlink("/var/log/smb");
	unlink("/var/log/nmb");
	
	eval("rm", "-rf", "/var/run/samba");

	logmessage("Samba Server", "smb daemon is stoped");

}
#endif	// RTCONFIG_SAMBASRV

#ifdef RTCONFIG_MEDIA_SERVER
#define MEDIA_SERVER_APP	"minidlna"

void start_dms(void)
{
	FILE *f;
	int port, pid;
	char dbdir[100], dmsdir[PATH_MAX];
	char *argv[] = { MEDIA_SERVER_APP, "-f", "/etc/"MEDIA_SERVER_APP".conf", "-R", NULL };
	static int once = 1;
	int i;
	char serial[18];
	char lan_ipaddr[16] = {0};
	char computer_name[MAXLEN_TCAPI_MSG] = {0};

	// if (getpid() != 1) {
		// notify_rc("start_dms");
		// return;
	// }

	if(!is_routing_enabled() && !is_lan_connected())
		set_invoke_later(INVOKELATER_DMS);

	if (tcapi_get_int(APPS_DATA, "dms_sas") == 0)
		once = 0;

	if (tcapi_get_int(APPS_DATA, "dms_enable") != 0) {

		if ((!once) && (tcapi_get_int(APPS_DATA, "dms_rescan") == 0)) {
			// no forced rescan
			argv[3] = NULL;
		}

		if ((f = fopen(argv[2], "w")) != NULL) {
			port = tcapi_get_int(APPS_DATA, "dms_port");
			// dir rule:
			// default: dmsdir=/tmp/mnt, dbdir=/var/cache/minidlna
			// after setting dmsdir, dbdir="dmsdir"/minidlna

			tcapi_get(APPS_DATA, "dms_dir", dmsdir);	// dmsdir = nvram_safe_get("dms_dir");
			if(!check_if_dir_exist(dmsdir)) 
				// dmsdir = nvram_default_get("dms_dir");
				strcpy(dmsdir, "/tmp/mnt");
			
			if(strcmp(dmsdir, "/tmp/mnt")==0)	// if(strcmp(dmsdir, nvram_default_get("dms_dir"))==0)
				strcpy(dbdir, "/var/cache/minidlna");
			else {
				if(dmsdir[strlen(dmsdir)-1]=='/') sprintf(dbdir, "%sminidlna", dmsdir);
				else sprintf(dbdir, "%s/minidlna", dmsdir);
			}
			mkdir_if_none(dbdir);

			// nvram_set("dms_dbcwd", dbdir);
			tcapi_set(APPS_DATA, "dms_dbcwd", dbdir);

			// strcpy(serial, nvram_safe_get("lan_hwaddr"));
			tcapi_get("Info_Ether", "mac", serial);
			if (strlen(serial))
				for (i = 0; i < strlen(serial); i++)
					serial[i] = tolower(serial[i]);

			tcapi_get("Info_Ether", "ip", lan_ipaddr);
			tcapi_get(SAMBA_DATA, "NetBiosName", computer_name);
			fprintf(f,
				"network_interface=%s\n"
				"port=%d\n"
				"friendly_name=%s\n"
//				"db_dir=%s/.db\n"
				"db_dir=%s\n"
				"enable_tivo=%s\n"
				"strict_dlna=%s\n"
				"presentation_url=http://%s:80/\n"
				"inotify=yes\n"
				"notify_interval=600\n"
				"album_art_names=Cover.jpg/cover.jpg/Thumb.jpg/thumb.jpg\n"
				"media_dir=%s\n"
				"serial=%s\n"
				"model_number=1"
				"\n",
				"br0",	// nvram_safe_get("lan_ifname"),
				(port < 0) || (port >= 0xffff) ? 0 : port,
				is_valid_hostname(computer_name) ? computer_name : get_productid(),	// nvram_get("computer_name") && is_valid_hostname(nvram_get("computer_name")) ? nvram_get("computer_name") : get_productid(),
				dbdir,
				tcapi_get_int(APPS_DATA, "dms_tivo") ? "yes" : "no",
				tcapi_get_int(APPS_DATA, "dms_stdlna") ? "yes" : "no",
				lan_ipaddr,//nvram_safe_get("lan_ipaddr"),
				dmsdir,
				serial
				);

			fclose(f);
		}

		/* start media server if it's not already running */
		if (pidof(MEDIA_SERVER_APP) <= 0) {
			if ((_eval(argv, NULL, 0, &pid) == 0) && (once)) {
				/* If we started the media server successfully, wait 1 sec
				 * to let it die if it can't open the database file.
				 * If it's still alive after that, assume it's running and
				 * disable forced once-after-reboot rescan.
				 */
				sleep(1);
				if (pidof(MEDIA_SERVER_APP) > 0)
					once = 0;
			}
		}
	}
	else killall_tk(MEDIA_SERVER_APP);
}

void stop_dms(void)
{
	// if (getpid() != 1) {
		// notify_rc("stop_dms");
		// return;
	// }

	// keep dms always run except for disabling it
	// killall_tk(MEDIA_SERVER_APP);
}


// it is called by rc only
void force_stop_dms(void)
{
	killall_tk(MEDIA_SERVER_APP);
}


void
write_mt_daapd_conf(char *servername)
{
	FILE *fp;
	char dmsdir[PATH_MAX];
	char username[MAXLEN_TCAPI_MSG];
	char password[MAXLEN_TCAPI_MSG];

	if (check_if_file_exist("/etc/mt-daapd.conf"))
		return;

	fp = fopen("/etc/mt-daapd.conf", "w");

	if (fp == NULL)
		return;

	tcapi_get(APPS_DATA, "dms_dir", dmsdir);// dmsdir = nvram_safe_get("dms_dir");
	if(!check_if_dir_exist(dmsdir)) 
		strcpy(dmsdir, "/tmp/mnt");// dmsdir = nvram_default_get("dms_dir");

	tcapi_get("Account_Entry0", "username", username);
	tcapi_get("Account_Entry0", "web_passwd", password);
#if 1
	fprintf(fp, "web_root /etc/web\n");
	fprintf(fp, "port 3689\n");
	fprintf(fp, "admin_pw %s\n", password);	// fprintf(fp, "admin_pw %s\n", nvram_safe_get("http_passwd"));
	fprintf(fp, "db_dir /var/cache/mt-daapd\n");
	fprintf(fp, "mp3_dir %s\n", dmsdir);
	fprintf(fp, "servername %s\n", servername);
	fprintf(fp, "runas %s\n", username);	//fprintf(fp, "runas %s\n", nvram_safe_get("http_username"));
	fprintf(fp, "extensions .mp3,.m4a,.m4p,.aac,.ogg\n");
	fprintf(fp, "rescan_interval 300\n");
	fprintf(fp, "always_scan 1\n");
	fprintf(fp, "compress 1\n");
#else
	fprintf(fp, "[general]\n");
	fprintf(fp, "web_root = /rom/mt-daapd/admin-root\n");
	fprintf(fp, "port = 3689\n");
	fprintf(fp, "admin_pw = %s\n", nvram_safe_get("http_passwd"));
	fprintf(fp, "db_type = sqlite3\n");
	fprintf(fp, "db_parms = /var/cache/mt-daapd\n");
	fprintf(fp, "mp3_dir = %s\n", "/mnt");
	fprintf(fp, "servername = %s\n", servername);
	fprintf(fp, "runas = %s\n", nvram_safe_get("http_username"));
	fprintf(fp, "extensions = .mp3,.m4a,.m4p\n");
	fprintf(fp, "scan_type = 2\n");
	fprintf(fp, "[plugins]\n");
	fprintf(fp, "plugin_dir = /rom/mt-daapd/plugins\n");
	fprintf(fp, "[scanning]\n");
	fprintf(fp, "process_playlists = 1\n");
	fprintf(fp, "process_itunes = 1\n");
	fprintf(fp, "process_m3u = 1\n");
#endif
	fclose(fp);
}

void
start_mt_daapd()
{
	char servername[32];
	char computer_name[MAXLEN_TCAPI_MSG]= {0};
	char lan_ipaddr[16] = {0};

	// if(getpid()!=1) {
		// notify_rc("start_mt_daapd");
		// return;
	// }

	if (!tcapi_match(APPS_DATA, "daapd_enable", "1"))	// if (nvram_invmatch("daapd_enable", "1"))
		return;

	tcapi_get(SAMBA_DATA, "NetBiosName", computer_name);
	if (is_valid_hostname(computer_name))	//if (is_valid_hostname(nvram_safe_get("computer_name")))
		strncpy(servername, computer_name, sizeof(servername));	//strncpy(servername, nvram_safe_get("computer_name"), sizeof(servername));
	else
		servername[0] = '\0';
	if(strlen(servername)==0) strncpy(servername, get_productid(), sizeof(servername));
	write_mt_daapd_conf(servername);

	if (is_routing_enabled())
	{
		system("mt-daapd -m");
		// doSystem("mDNSResponder %s thehost %s _daap._tcp. 3689 &", nvram_safe_get("lan_ipaddr"), servername);
		tcapi_get("Info_Ether", "ip", lan_ipaddr);
		doSystem("mDNSResponder %s thehost %s _daap._tcp. 3689 &", lan_ipaddr, servername);
	}
	else
		system("mt-daapd");

	if (pids("mt-daapd"))
	{
		logmessage("iTunes Server", "daemon is started");

		return;
	}

	return;
}

void
stop_mt_daapd()
{
	// if(getpid()!=1) {
		// notify_rc("stop_mt_daapd");
		// return;
	// }

	if (!pids("mDNSResponder") && !pids("mt-daapd"))
		return;

	if (pids("mDNSResponder"))
		system("killall mDNSResponder");

	if (pids("mt-daapd"))
		system("killall -SIGKILL mt-daapd");

	unlink("/etc/mt-daapd.conf");

	logmessage("iTunes", "daemon is stoped");
}
#endif	// RTCONFIG_MEDIA_SERVER

// -------------

// !!TB - webdav

#ifdef RTCONFIG_WEBDAV

#define	ACC_PW_MAX_LEN	20

void write_webdav_permissions()
{
	FILE *fp;
	int acc_num = 0, i;
	char *nv, *nvp, *b;
	char *tmp_ascii_user, *tmp_ascii_passwd;

	/* write /tmp/lighttpd/permissions */
	fp = fopen("/tmp/lighttpd/permissions", "w");
	if (fp==NULL) return;

	// acc_num = atoi(nvram_safe_get("acc_num"));
	acc_num = tcapi_get_int(SAMBA_DATA, "acc_num");
	if(acc_num < 0)
		acc_num = 0;

	nv = (char *)malloc(sizeof(char) * (ACC_PW_MAX_LEN+1) * acc_num * 3 * 2);
	if(nv == NULL) {
		usb_dbg("Can't malloc \"nv\".\n");
		return;
	}

	if(tcapi_get(SAMBA_DATA, "acc_list", nv)) {
		usb_dbg("Unexcept error. No acc_list.\n");
		free(nv);
		return;
	}
	nvp = nv;//nv = nvp = strdup(nvram_safe_get("acc_list"));
	i = 0;
	if(nv && strlen(nv) > 0){
		while((b = strsep(&nvp, "<")) != NULL){
			if(vstrsep(b, ">", &tmp_ascii_user, &tmp_ascii_passwd) != 2)
				continue;

			char char_user[64], char_passwd[64];

			memset(char_user, 0, 64);
			ascii_to_char_safe(char_user, tmp_ascii_user, 64);
			memset(char_passwd, 0, 64);
			ascii_to_char_safe(char_passwd, tmp_ascii_passwd, 64);

			fprintf(fp, "%s:%s\n", char_user, char_passwd);

			if(++i >= acc_num)
				break;
		}
	}
	if(nv)
		free(nv);

	fclose(fp);
}

void write_webdav_server_pem()
{
	// unsigned long long sn;
	// char t[32];
#ifdef RTCONFIG_HTTPS
	if(f_exists("/etc/server.pem"))
		system("cp -f /etc/server.pem /tmp/lighttpd/");
#endif
	if(!f_exists("/tmp/lighttpd/server.pem")){
		// f_read("/dev/urandom", &sn, sizeof(sn));
		// sprintf(t, "%llu", sn & 0x7FFFFFFFFFFFFFFFULL);
		// eval("/usr/script/gencert.sh", t);
		eval("/usr/script/gencert.sh");

		system("cp -f /etc/server.pem /tmp/lighttpd/");
	}
}

void start_webdav()	// added by Vanic
{
	// if(getpid()!=1) {
		// notify_rc("start_webdav");
		// return;
	// }

	//static char *lighttpd_monitor_argv[] = { "lighttpd-monitor", NULL, NULL };

	// if(nvram_get_int("sw_mode") != SW_MODE_ROUTER) return;

	if (tcapi_get_int(WEBDAV, "webdav_aidisk") || tcapi_get_int(WEBDAV, "webdav_proxy"))
		tcapi_set(WEBDAV, "enable_webdav", "1");
	else if (!tcapi_get_int(WEBDAV, "webdav_aidisk") && !tcapi_get_int(WEBDAV, "webdav_proxy") && tcapi_get_int(WEBDAV, "enable_webdav")) {
		tcapi_set(WEBDAV, "webdav_aidisk", "1");
		tcapi_set(WEBDAV, "webdav_proxy", "1");
	}
	// if (nvram_get_int("webdav_aidisk") || nvram_get_int("webdav_proxy"))
		// nvram_set("enable_webdav", "1");
	// else if (!nvram_get_int("webdav_aidisk") && !nvram_get_int("webdav_proxy") && nvram_get_int("enable_webdav")) 
	// {
		// // enable both when enable_webdav by other apps
		// nvram_set("webdav_aidisk", "1");
		// nvram_set("webdav_proxy", "1");
	// }
	
	// if (nvram_match("enable_webdav", "0")) return;
	if (tcapi_match(WEBDAV, "enable_webdav", "0")) return;

	/* WebDav directory */
	mkdir_if_none("/tmp/lighttpd");
	mkdir_if_none("/tmp/lighttpd/uploads");
	chmod("/tmp/lighttpd/uploads", 0777);
	mkdir_if_none("/tmp/lighttpd/www");
	chmod("/tmp/lighttpd/www", 0777);

	/* tmp/lighttpd/permissions */
	write_webdav_permissions();

	/* WebDav SSL support */
	write_webdav_server_pem();

	/* write WebDav configure file*/
	system("/userfs/bin/write_webdav_conf");

	if (!f_exists("/tmp/lighttpd.conf")) return;

	if (!pids("lighttpd")){
		system("/usr/sbin/lighttpd -f /tmp/lighttpd.conf -D &");
	}
	if (!pids("lighttpd-monitor")){
		system("/usr/sbin/lighttpd-monitor &");
	   //_eval(lighttpd_monitor_argv, NULL, 0, NULL);
	}

	if (pids("lighttpd"))
		logmessage("WEBDAV server", "daemon is started");
}

void stop_webdav(void)
{
	// if (getpid() != 1) {
		// notify_rc("stop_webdav");
		// return;
	// }
	
	if (pids("lighttpd-monitor")){
		kill_pidfile_tk("/tmp/lighttpd/lighttpd-monitor.pid");
		unlink("/tmp/lighttpd/lighttpd-monitor.pid");
	}

	if (pids("lighttpd")){
		kill_pidfile_tk("/tmp/lighttpd/lighttpd.pid");
		// charles: unlink lighttpd.conf will cause lighttpd error
		//	we should re-write lighttpd.conf
		system("/userfs/bin/write_webdav_conf");

		unlink("/tmp/lighttpd/lighttpd.pid");
	}

	if (pids("lighttpd-arpping")){
		kill_pidfile_tk("/tmp/lighttpd/lighttpd-arpping.pid");
		unlink("/tmp/lighttpd/lighttpd-arpping.pid");
	}

	logmessage("WEBDAV Server", "daemon is stoped");
}
#endif	// RTCONFIG_WEBDAV

#ifdef RTCONFIG_CLOUDSYNC
void start_cloudsync(){
	char word[PATH_MAX], *next_word;
	char cloud_setting[MAXLEN_TCAPI_MSG];
	char *b, *nvp;
	int type = 0, enable = 0;
	char username[64], sync_dir[PATH_MAX];
	int count;
	char cloud_token[PATH_MAX];
	char mounted_path[PATH_MAX], *ptr, *other_path;
	int pid;
	char *cmd1_argv[] = { "/userfs/bin/inotify", NULL };
	char *cmd2_argv[] = { "/userfs/bin/asuswebstorage", NULL };
	char *cmd3_argv[] = { "/bin/touch", cloud_token, NULL };
	char *cmd4_argv[] = { "/userfs/bin/webdav_client", NULL };
	char *cmd5_argv[] = { "/userfs/bin/dropbox_client", NULL };
	char *cmd6_argv[] = { "/userfs/bin/ftpclient", NULL};
	char *cmd7_argv[] = { "/userfs/bin/sambaclient", NULL};
	char *cmd8_argv[] = { "/userfs/bin/usbclient", NULL};
	// if(getpid()!=1) {
		// notify_rc("start_cloudsync");
		// return;
	// }

	// if(nvram_get_int("sw_mode") != SW_MODE_ROUTER) return;

	// if(nvram_match("enable_cloudsync", "0")){
	if(tcapi_match(CLOUDSYNC, "enable_cloudsync", "0")){
		logmessage("Cloudsync client", "manually disabled all rules");
		return;
	}

	// cloud_setting = nvram_safe_get("cloud_sync");

	// nv = nvp = strdup(nvram_safe_get("cloud_sync"));
	nvp = cloud_setting;
	// if(nv){
	if(!tcapi_get(CLOUDSYNC, "cloud_sync", cloud_setting)) {
		while((b = strsep(&nvp, "<")) != NULL){
			count = 0;
			foreach_62(word, b, next_word){
				switch(count){
					case 0: // type
						type = atoi(word);
						break;
				}
				++count;
			}

			if(type == 1){
				if(!pids("inotify"))
					_eval(cmd1_argv, NULL, 0, &pid);

				if(!pids("webdav_client"))
					_eval(cmd4_argv, NULL, 0, &pid);

				if(pids("inotify") && pids("webdav_client"))
					logmessage("Webdav client", "daemon is started");
			}
			else if(type == 3){
				if(!pids("inotify"))
					_eval(cmd1_argv, NULL, 0, &pid);

				if(!pids("dropbox_client")){
					_eval(cmd5_argv, NULL, 0, &pid);
					sleep(2); // wait dropbox_client.
				}

				if(pids("inotify") && pids("dropbox_client"))
					logmessage("dropbox client", "daemon is started");
			}
			else if(type == 2){
				if(!pids("inotify"))
					_eval(cmd1_argv, NULL, 0, &pid);

				if(!pids("ftpclient")){
					_eval(cmd6_argv, NULL, 0, &pid);
					sleep(2); // wait ftpclient.
				}

				if(pids("inotify") && pids("ftpclient"))
					logmessage("ftp client", "daemon is started");
			}
			else if(type == 4){
				if(!pids("inotify"))
					_eval(cmd1_argv, NULL, 0, &pid);

				if(!pids("sambaclient")){
					_eval(cmd7_argv, NULL, 0, &pid);
					sleep(2); // wait sambaclient.
				}

				if(pids("inotify") && pids("sambaclient"))
					logmessage("sambaclient", "daemon is started");
			}
			else if(type == 5){
				if(!pids("inotify"))
					_eval(cmd1_argv, NULL, 0, &pid);

				if(!pids("usbclient")){
					_eval(cmd8_argv, NULL, 0, &pid);
					sleep(2); // wait usbclient.
				}

				if(pids("inotify") && pids("usbclient"))
					logmessage("usbclient", "daemon is started");
			}
			else{
				count = 0;
				foreach_62(word, b, next_word){
					switch(count){
						case 0: // type
							type = atoi(word);
							break;
						case 1: // username
							memset(username, 0, 64);
							strncpy(username, word, 64);
							break;
						case 5: // dir
							memset(sync_dir, 0, PATH_MAX);
							strncpy(sync_dir, word, PATH_MAX);
							break;
						case 6: // enable
							enable = atoi(word);
							break;
					}

					++count;
				}
				if(!enable){
					logmessage("Cloudsync client", "manually disabled");
					return;
				}

				ptr = sync_dir+strlen(POOL_MOUNT_ROOT)+1;
				if((other_path = strchr(ptr, '/')) != NULL){
					ptr = other_path;
					++other_path;
				}
				else
					ptr = "";

				memset(mounted_path, 0, PATH_MAX);
				strncpy(mounted_path, sync_dir, (strlen(sync_dir)-strlen(ptr)));

				FILE *fp;
				char check_target[PATH_MAX], line[PATH_MAX];
				int got_mount = 0;

				memset(check_target, 0, PATH_MAX);
				sprintf(check_target, " %s ", mounted_path);

				if((fp = fopen(MOUNT_FILE, "r")) == NULL){
					logmessage("Cloudsync client", "Could read the disk's data");
					return;
				}

				while(fgets(line, sizeof(line), fp) != NULL){
					if(strstr(line, check_target)){
						got_mount = 1;
						break;
					}
				}
				fclose(fp);

				if(!got_mount){
					logmessage("Cloudsync client", "The specific disk isn't existed");
					return;
				}

				if(strlen(sync_dir))
					mkdir_if_none(sync_dir);

				memset(cloud_token, 0, PATH_MAX);
				sprintf(cloud_token, "%s/.__cloudsync_%d_%s.txt", mounted_path, type, username);

				_eval(cmd3_argv, NULL, 0, NULL);

				if(!pids("inotify"))
					_eval(cmd1_argv, NULL, 0, &pid);

				if(!pids("asuswebstorage"))
					_eval(cmd2_argv, NULL, 0, &pid);
				sleep(2); // wait asuswebstorage.

				if(pids("inotify") && pids("asuswebstorage"))
					logmessage("Cloudsync client", "daemon is started");
			}
		}
		// free(nv);
	}
}

void stop_cloudsync()
{
	// if(getpid()!=1) {
		// notify_rc("stop_cloudsync");
		// return;
	// }

	if(pids("inotify"))
		killall_tk("inotify");

	if(pids("webdav_client"))
		killall_tk("webdav_client");

	if(pids("asuswebstorage"))
		killall_tk("asuswebstorage");

	if(pids("dropbox_client"))
		killall_tk("dropbox_client");

	if(pids("ftpclient"))
		killall_tk("ftpclient");

	if(pids("sambaclient"))
		killall_tk("sambaclient");

	if(pids("usbclient"))
		killall_tk("usbclient");

	logmessage("Cloudsync client and Webdav_client and dropbox_client and  ftpclient and  sambaclient and  usbclient", "daemon is stoped");
}
#endif

#ifdef RTCONFIG_USB
void start_nas_services(int force)
{
#if 0
	if(!force && getpid() != 1){
		notify_rc_after_wait("start_nasapps");
		return;
	}
#endif

	if(check_if_dir_exist("/mnt"))
		eval("/usr/sbin/usbtest.sh");

	if(!check_if_dir_empty("/mnt"))
	{
#ifdef RTCONFIG_WEBDAV
		// webdav still needed if no disk is mounted
		start_webdav();
#else
		if(f_exists("/opt/etc/init.d/S50aicloud"))
			system("sh /opt/etc/init.d/S50aicloud scan");
#endif
		return;
	}

	create_passwd();
#ifdef RTCONFIG_SAMBASRV
	start_samba();
#endif

if (nvram_match("asus_mfg", "0")) {
#ifdef RTCONFIG_FTP
	start_ftpd();
#endif
#ifdef RTCONFIG_MEDIA_SERVER
	start_dms();
	start_mt_daapd();
#endif
#ifdef RTCONFIG_WEBDAV
	//start_webdav();
#endif
}
#ifdef RTCONFIG_TIMEMACHINE
	start_timemachine();
#endif
}

void stop_nas_services(int force)
{
#if 0
	if(!force && getpid() != 1){
		notify_rc_after_wait("stop_nasapps");
		return;
	}
#endif

#ifdef RTCONFIG_MEDIA_SERVER
	force_stop_dms();
	stop_mt_daapd();
#endif
#ifdef RTCONFIG_FTP
	stop_ftpd();
#endif
#ifdef RTCONFIG_SAMBASRV
	stop_samba();
#endif
#ifdef RTCONFIG_WEBDAV
	//stop_webdav();
#endif
#ifdef RTCONFIG_TIMEMACHINE
	stop_timemachine();
#endif
}

void restart_nas_services(int stop, int start)
{
	int fd = file_lock("usbnas");

	/* restart all NAS applications */
	if (stop)
		stop_nas_services(0);
	if (start)
		start_nas_services(0);
	file_unlock(fd);
}


void restart_sambaftp(int stop, int start)
{
	int fd = file_lock("sambaftp");
	/* restart all NAS applications */
	if (stop) {
#ifdef RTCONFIG_SAMBASRV
		stop_samba();
#endif
#ifdef RTCONFIG_FTP
		stop_ftpd();
#endif
#ifdef RTCONFIG_WEBDAV
		stop_webdav();
#endif
	}
	
	if (start) {
#ifdef RTCONFIG_SAMBA_SRV
		create_passwd();
		start_samba();
#endif
#ifdef RTCONFIG_FTP
		start_ftpd();
#endif
#ifdef RTCONFIG_WEBDAV
		start_webdav();
#endif
	}
	file_unlock(fd);
}

int ejusb_main(int argc, char *argv[])
{
	disk_info_t *disk_list, *disk_info;
	partition_info_t *partition_info;
	char nvram_name[32], device_name[8], devpath[16];
	int got_usb_port;
	int restart_nasapps = 1;

	if(argc != 2 && argc != 3){
		printf("Usage: ejusb [disk_port] [0|1]\n");
		return 0;
	}

	if (argc == 3)
		restart_nasapps = atoi(argv[2]);

	got_usb_port = atoi(argv[1]);
	if(got_usb_port < 1 || got_usb_port > 3){
		printf("Usage: ejusb [disk_port] [0|1]\n");
		return 0;
	}

	disk_list = read_disk_data();
	if(disk_list == NULL){
		printf("Can't get any disk's information.\n");
		return 0;
	}

	memset(nvram_name, 0, 32);
	sprintf(nvram_name, "usb_path%d_act", got_usb_port);
	memset(device_name, 0, 8);
	tcapi_get(USB_DATA, nvram_name, device_name);
	// strcpy(device_name, nvram_safe_get(nvram_name));

	for(disk_info = disk_list; disk_info != NULL; disk_info = disk_info->next)
		if(!strcmp(disk_info->device, device_name))
			break;
	if(disk_info == NULL){
		printf("Can't find the information of the device: %s\n", device_name);
		free_disk_data(&disk_list);
		return 0;
	}

	memset(nvram_name, 0, 32);
	sprintf(nvram_name, "usb_path%d_removed", got_usb_port);
	// nvram_set(nvram_name, "1");
	tcapi_set(USB_DATA, nvram_name, "1");

	for(partition_info = disk_info->partitions; partition_info != NULL; partition_info = partition_info->next){
		// if(partition_info->mount_point != NULL){	//if support swap partition, no mount_point for swap
			memset(devpath, 0, 16);
			sprintf(devpath, "/dev/%s", partition_info->device);

			umount_partition(devpath, 0, NULL, NULL, EFH_HP_REMOVE);
		// }
	}
	free_disk_data(&disk_list);

	if (restart_nasapps) {
		_dprintf("restart_nas_services(%d): test 7.\n", getpid());
		restart_nas_services(1, 1);
		// notify_rc_after_wait("restart_nasapps");
	} else {
		_dprintf("restart_nas_services(%d) is skipped: test 7.\n", getpid());
	}
	
	tcapi_save();	//Andy Chiu, 2015/03/19
	return 0;
}

//Andy Chiu, 2015/04/23. Add for disk utility
#ifdef ASUS_DISK_UTILITY
static int stop_diskscan()
{
	//return nvram_get_int("diskmon_force_stop");
	return tcapi_get_int("USB_Entry", "diskmon_force_stop");
}

static void start_diskscan(int usb_port)
{
	disk_info_t *disk_list, *disk_info;
	partition_info_t *partition_info;

	char devpath[16], buf[64], buf2[64];

	cprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
	if(stop_diskscan())
		return;

	if(-1 == usb_port)
	{
		usb_port = tcapi_get_int("USB_Entry", "scan_port");
	}

	//get disk list
	disk_list = read_disk_data();
	if(disk_list == NULL){
		cprintf("Can't get any disk's information.\n");
		return;
	}

	for(disk_info = disk_list; disk_info != NULL; disk_info = disk_info->next){
		//check port and type. only storage could do disk utility.
		sprintf(buf, "usb_path%d", usb_port);
		if(disk_info->port && atoi(disk_info->port) == usb_port && tcapi_match("USB_Entry", buf, "storage"))
		{
			//check all partitions.
			for(partition_info = disk_info->partitions; partition_info != NULL; partition_info = partition_info->next)
			{
				if(partition_info->mount_point == NULL)
				{
					cprintf("Skip to scan %s: It can't be mounted.\n");
					continue;
				}

				cprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
				
				// there's some problem with fsck.ext4.
				if(!strcmp(partition_info->file_system, "ext4"))
					continue;

				if(stop_diskscan())
					goto stop_scan;
				
				// umount partition and stop USB apps.
				cprintf("disk_monitor: umount partition %s...\n", partition_info->device);
				diskmon_status(DISKMON_UMOUNT);
				sprintf(devpath, "/dev/%s", partition_info->device);
				umount_partition(devpath, 0, NULL, NULL, EFH_HP_REMOVE);

				if(stop_diskscan())
					goto stop_scan;

				// scan partition.
				eval("mount"); /* what for ??? */
				cprintf("disk_monitor: scan partition %s...\n", partition_info->device);
				diskmon_status(DISKMON_SCAN);
				cprintf("[%s, %d]<%s, %s>\n", __FUNCTION__, __LINE__, partition_info->file_system, devpath);
				//eval("app_fsck.sh", partition_info->file_system, devpath);
				sprintf(buf2, "app_fsck.sh %s %s", partition_info->file_system, devpath);
				system(buf2);

				if(stop_diskscan())
					goto stop_scan;

				// re-mount partition.
				cprintf("disk_monitor: re-mount partition %s...\n", partition_info->device);
				diskmon_status(DISKMON_REMOUNT);
				mount_partition(devpath, -3, NULL, partition_info->device, EFH_HP_ADD);

				start_nas_services(1);
			}
		}
	}

	free_disk_data(&disk_list);
	// finish & restart USB apps.
	cprintf("disk_monitor: done.\n");
	diskmon_status(DISKMON_FINISH);
	return;

stop_scan:
	free_disk_data(&disk_list);
	diskmon_status(DISKMON_FORCE_STOP);
}

#define NO_SIG -1

static int diskmon_signal = NO_SIG;

static void diskmon_sighandler(int sig)
{
	cprintf("[%s, %d]sig(%d)\n", __FUNCTION__, __LINE__, sig);
#if 1
	switch(sig) {
		case SIGTERM:
			cprintf("disk_monitor: Finish!\n");
			logmessage("disk_monitor", "Finish");
			unlink("/var/run/disk_monitor.pid");
			diskmon_signal = sig;
			exit(0);
		case SIGUSR1:
			//logmessage("disk_monitor", "Check status: %d.", diskmon_status(-1));
			//cprintf("disk_monitor: Check status: %d.\n", diskmon_status(-1));
			diskmon_signal = sig;
			break;
		case SIGUSR2:
			logmessage("disk_monitor", "Scan manually...");
			cprintf("disk_monitor: Scan manually...\n");
			//diskmon_status(DISKMON_START);
			start_diskscan(-1);
			sleep(10);
			//diskmon_status(DISKMON_IDLE);
			diskmon_signal = sig;
			break;
		case SIGALRM:
			logmessage("disk_monitor", "Got SIGALRM...");
			cprintf("disk_monitor: Got SIGALRM...\n");
			diskmon_signal = sig;
			break;
	}
#endif
}

void start_diskmon(void)
{
	char *diskmon_argv[] = { "disk_monitor", NULL };
	pid_t pid;

	cprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
	_eval(diskmon_argv, NULL, 0, &pid);
	cprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
}

void stop_diskmon(void)
{
	cprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
	killall_tk("disk_monitor");
}

int first_alarm = 1;

int diskmon_main(int argc, char *argv[])
{
	FILE *fp;
	sigset_t mask;
	int diskmon_freq = DISKMON_FREQ_DISABLE;
	time_t now;
	struct tm local;
	char *nv, *nvp;
	char *set_day, *set_week, *set_hour;
	int val_day[2] = {0, 0}, val_hour[2] = {0, 0};
	int wait_second[2] = {0, 0}, wait_hour = 0;
	int diskmon_alarm_sec = 0;

	cprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
	fp = fopen("/var/run/disk_monitor.pid", "w");
	if(fp != NULL) {
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	cprintf("disk_monitor: starting...\n");
	//diskmon_status(DISKMON_IDLE);

	nvram_set_int("diskmon_force_stop", 0);

	signal(SIGTERM, diskmon_sighandler);
	signal(SIGUSR1, diskmon_sighandler);
	signal(SIGUSR2, diskmon_sighandler);
	signal(SIGALRM, diskmon_sighandler);

	sigfillset(&mask);
	sigdelset(&mask, SIGTERM);
	sigdelset(&mask, SIGUSR1);
	sigdelset(&mask, SIGUSR2);
	sigdelset(&mask, SIGALRM);

	int diskmon_enable, port_num;
	char word[PATH_MAX], *next;
	char buf[64], attr[32], buf2[64];
	
	diskmon_enable = 0;
	port_num = 0;
	//parser ehci_ports to confirm the number of usb ports
	memset(buf, 0, sizeof(buf));
	tcapi_get("USB_Entry", "ehci_ports", buf);
	
	foreach(word, buf, next)
	{
		memset(attr, 0, 32);
		sprintf(attr, "usb_path%d_diskmon_freq", (port_num+1));

		//get freq
		diskmon_freq = tcapi_get_int("USB_Entry", attr);
		if(diskmon_freq == DISKMON_FREQ_DISABLE){
			++port_num;
			continue;
		}

		diskmon_enable += 1<<port_num;
		cprintf("disk_monitor: diskmon_enable=%d.\n", diskmon_enable);

		memset(attr, 0, 32);
		sprintf(attr, "usb_path%d_diskmon_freq_time", (port_num+1));

		memset(buf2, 0, sizeof(buf2));
		nvp = tcapi_get_string("USB_Entry", attr, buf2);
		if(!nvp || strlen(buf2) <= 0){
			cprintf("disk_monitor: Without setting the running time at the port %d!\n", (port_num+1));
			++port_num;
			continue;
		}

		if((vstrsep(nvp, ">", &set_day, &set_week, &set_hour) != 3)){
			cprintf("disk_monitor: Without the correct running time at the port %d!\n", (port_num+1));
			++port_num;
			continue;
		}

		val_hour[port_num] = atoi(set_hour);
		if(diskmon_freq == DISKMON_FREQ_MONTH)
			val_day[port_num] = atoi(set_day);
		else if(diskmon_freq == DISKMON_FREQ_WEEK)
			val_day[port_num] = atoi(set_week);
		else if(diskmon_freq == DISKMON_FREQ_DAY)
			val_day[port_num] = -1;
		cprintf("disk_monitor: Port %d: val_day=%d, val_hour=%d.\n", port_num, val_day[port_num], val_hour[port_num]);

		++port_num;
	}

	while(1){
		time(&now);
		localtime_r(&now, &local);
		cprintf("disk_monitor: day=%d, week=%d, time=%d:%d.\n", local.tm_mday, local.tm_wday, local.tm_hour, local.tm_min);

		if(diskmon_signal == SIGUSR2){
			cprintf("disk_monitor: wait more %d seconds and avoid to scan too often.\n", DISKMON_SAFE_RANGE*60);
			diskmon_alarm_sec = DISKMON_SAFE_RANGE*60;
		}
		else if(first_alarm || diskmon_signal == SIGALRM){
			cprintf("disk_monitor: decide if scan the target...\n");
			diskmon_alarm_sec = 0;
			port_num = 0;

			memset(buf, 0, sizeof(buf));
			tcapi_get("USB_Entry", "ehci_ports", buf);

			foreach(word, buf, next){
				if(local.tm_min <= DISKMON_SAFE_RANGE){
					if(val_hour[port_num] == local.tm_hour){
						if((diskmon_freq == DISKMON_FREQ_MONTH && val_day[port_num] == local.tm_mday)
								|| (diskmon_freq == DISKMON_FREQ_WEEK && val_day[port_num] == local.tm_wday)
								|| (diskmon_freq == DISKMON_FREQ_DAY)){
							cprintf("disk_monitor: Running...\n");
							// Running!!
							diskmon_status(DISKMON_START);
							start_diskscan(port_num + 1);
							sleep(10);
							diskmon_status(DISKMON_IDLE);
						}

						wait_hour = DISKMON_DAY_HOUR;
					}
					else if(val_hour[port_num] > local.tm_hour)
						wait_hour = val_hour[port_num]-local.tm_hour;
					else // val_hour < local.tm_hour
						wait_hour = 23-local.tm_hour+val_hour[port_num];

					wait_second[port_num] = wait_hour*DISKMON_HOUR_SEC;
				}
				else
					wait_second[port_num] = (60-local.tm_min)*60;
				cprintf("disk_monitor: %d: wait_second=%d...\n", port_num, wait_second[port_num]);

				if(diskmon_alarm_sec == 0 || diskmon_alarm_sec > wait_second[port_num])
					diskmon_alarm_sec = wait_second[port_num];

				++port_num;
			}
		}

		if(first_alarm || diskmon_signal == SIGUSR2 || diskmon_signal == SIGALRM){
			if(first_alarm)
				first_alarm = 0;

			cprintf("disk_monitor: wait_second=%d...\n", diskmon_alarm_sec);
			alarm(diskmon_alarm_sec);
		}

		cprintf("disk_monitor: Pause...\n\n");
		diskmon_signal = NO_SIG;
		sigsuspend(&mask);
	}
	unlink("/var/run/disk_monitor.pid");
	return 0;
}

#endif

#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
int start_app(){
	char cmd[PATH_MAX];
	// char *apps_dev = nvram_safe_get("apps_dev");
	// char *apps_mounted_path = nvram_safe_get("apps_mounted_path");
	char apps_dev[MAXLEN_TCAPI_MSG] = {0};
	char apps_mounted_path[MAXLEN_TCAPI_MSG] = {0};

	tcapi_get(APPS_DATA, "apps_dev", apps_dev);
	tcapi_get(APPS_DATA, "apps_mounted_path", apps_mounted_path);

	if(strlen(apps_dev) <= 0 || strlen(apps_mounted_path) <= 0)
		return -1;

	memset(cmd, 0, PATH_MAX);
	sprintf(cmd, "/opt/.asusrouter %s %s", apps_dev, apps_mounted_path);
	system(cmd);
	tcapi_save();	//Andy Chiu, 2015/03/18
	return 0;
}

int stop_app(){
	// char *apps_dev = nvram_safe_get("apps_dev");
	// char *apps_mounted_path = nvram_safe_get("apps_mounted_path");
	char apps_dev[MAXLEN_TCAPI_MSG] = {0};
	char apps_mounted_path[MAXLEN_TCAPI_MSG] = {0};

	tcapi_get(APPS_DATA, "apps_dev", apps_dev);
	tcapi_get(APPS_DATA, "apps_mounted_path", apps_mounted_path);

	if(strlen(apps_dev) <= 0 || strlen(apps_mounted_path) <= 0)
		return -1;

	system("app_stop.sh");
	sync();

	return 0;
}

void usb_notify(){
	char target_dir[128], target[128], buf[16];
	DIR *dp;
	struct dirent *entry;

	memset(target_dir, 0, 128);
	sprintf(target_dir, "%s/%s", NOTIFY_DIR, NOTIFY_TYPE_USB);
	if(!check_if_dir_exist(target_dir))
		return;

	if(!(dp = opendir(target_dir)))
		return;

	while((entry = readdir(dp)) != NULL){
		if(entry->d_name[0] == '.')
			continue;

		memset(target, 0, 128);
		sprintf(target, "%s/%s", target_dir, entry->d_name);

		if(!pids(entry->d_name)){
			unlink(target);
			continue;
		}

		f_read_string(target, buf, 16);

		killall(entry->d_name, atoi(buf));
	}
	closedir(dp);
}
#endif
#endif // RTCONFIG_USB

#ifdef RTCONFIG_WEBDAV
#define DEFAULT_WEBDAVPROXY_RIGHT 0

int find_webdav_right(char *account)
{
	char *nv, *nvp, *b;
	char *acc, *right;
	int ret;

	nv = (char *)malloc(sizeof(char) * MAXLEN_TCAPI_MSG);
	if(nv == NULL) {
		usb_dbg("Can't malloc \"nv\".\n");
		return -1;
	}

	if(tcapi_get(SAMBA_DATA, "acc_webdavproxy", nv)) {
		usb_dbg("Unexcept error. No acc_webdavproxy.\n");
		free(nv);
		return -1;
	}
	nvp = nv;//nv = nvp = strdup(nvram_safe_get("acc_webdavproxy"));
	ret = DEFAULT_WEBDAVPROXY_RIGHT;

	if(nv) {
		while ((b = strsep(&nvp, "<")) != NULL) {
			if((vstrsep(b, ">", &acc, &right) != 2)) continue;

			if(strcmp(acc, account)==0) {
				ret = atoi(right);
				break;
			}
		}
		free(nv);
	}

	return ret;
}

void webdav_account_default(void)
{
	char *nv, *nvp, *b;
	char *accname, *accpasswd;
	int right;
	char new[256];
	int i;

	nv = (char *)malloc(sizeof(char) * MAXLEN_TCAPI_MSG);
	if(nv == NULL) {
		usb_dbg("Can't malloc \"nv\".\n");
		return;
	}

	if(tcapi_get(SAMBA_DATA, "acc_list", nv)) {
		usb_dbg("Unexcept error. No acc_list.\n");
		free(nv);
		return;
	}
	nvp = nv;//nv = nvp = strdup(nvram_safe_get("acc_list"));
	i = 0;
	strcpy(new, "");

	if(nv) {
		i=0;
		while ((b = strsep(&nvp, "<")) != NULL) {
			if((vstrsep(b, ">", &accname, &accpasswd) != 2)) continue;

			right = find_webdav_right(accname);
				
			if(i==0) sprintf(new, "%s>%d", accname, right);
			else sprintf(new, "%s<%s>%d", new, accname, right);
			i++;
		}
		free(nv);
		// nvram_set("acc_webdavproxy", new);
		tcapi_set(WEBDAV, "acc_webdavproxy", new);
	}
}
#endif

#ifdef TCSUPPORT_SWAP_FILE
int create_swapfile(char *mountpoint)
{
	u64 total_size, free_size;
	struct statfs fsbuf;
	struct stat st;
	char swap_file[PATH_MAX] = {0};
	char command[PATH_MAX] = {0};
	int create_flag = 1;
	int swap_existed = 0;

	if(statfs(mountpoint, &fsbuf))
		return -ENOENT;

	//check swap folder
	sprintf(swap_file, "%s/%s", mountpoint, SWAP_FOLDER);
	if( stat(swap_file, &st) ) {
		if(mkdir(swap_file, S_IRWXU))
			return -1;
	}
	else if(!S_ISDIR(st.st_mode)) {
		unlink(swap_file);
		if(mkdir(swap_file, S_IRWXU))
			return -1;
	}
	//check swap file
	sprintf(swap_file, "%s/%s/%s", mountpoint, SWAP_FOLDER, SWAP_FILENAME);
	if( !stat(swap_file, &st) ) {
		if(S_ISDIR(st.st_mode)) {
			if(rmdir(swap_file))
				return -1;
		}
		else if(S_ISREG(st.st_mode))
			swap_existed = 1;
		else
			return -1;
	}

	usb_dbg("%s\n", swap_file);

	total_size = (u64)((u64)fsbuf.f_blocks*(u64)fsbuf.f_bsize)/1048576;
	free_size = (u64)((u64)fsbuf.f_bfree*(u64)fsbuf.f_bsize)/1048576;
	usb_dbg("%llu %llu\n", total_size, free_size);

	if(total_size > USB_DISK_SIZE_4G) {
		if(free_size > 256) {
			if(swap_existed && (st.st_size >= SWAP_FILE_SIZE_256M))
				create_flag = 0;
			else
				sprintf(command, SWAP_DD_COMMAND, mountpoint, fsbuf.f_bsize, SWAP_FILE_SIZE_256M/fsbuf.f_bsize);
		}
		else if(free_size > 128) {
			if(swap_existed && (st.st_size >= SWAP_FILE_SIZE_128M))
				create_flag = 0;
			else
				sprintf(command, SWAP_DD_COMMAND, mountpoint, fsbuf.f_bsize, SWAP_FILE_SIZE_128M/fsbuf.f_bsize);
		}
		else if(free_size > 64) {
			if(swap_existed && (st.st_size >= SWAP_FILE_SIZE_64M))
				create_flag = 0;
			else
				sprintf(command, SWAP_DD_COMMAND, mountpoint, fsbuf.f_bsize, SWAP_FILE_SIZE_64M/fsbuf.f_bsize);
		}
		else
			return -ENOSPC;
	}
	else if(total_size > USB_DISK_SIZE_2G) {
		if(free_size > 128) {
			if(swap_existed && (st.st_size >= SWAP_FILE_SIZE_128M))
				create_flag = 0;
			else
				sprintf(command, SWAP_DD_COMMAND, mountpoint, fsbuf.f_bsize, SWAP_FILE_SIZE_128M/fsbuf.f_bsize);
		}
		else if(free_size > 64) {
			if(swap_existed && (st.st_size >= SWAP_FILE_SIZE_64M))
				create_flag = 0;
			else
				sprintf(command, SWAP_DD_COMMAND, mountpoint, fsbuf.f_bsize, SWAP_FILE_SIZE_64M/fsbuf.f_bsize);
		}
		else
			return -ENOSPC;
	}
	else if(total_size > USB_DISK_SIZE_1G) {
		if(free_size > 64) {
			if(swap_existed && (st.st_size >= SWAP_FILE_SIZE_64M))
				create_flag = 0;
			else
				sprintf(command, SWAP_DD_COMMAND, mountpoint, fsbuf.f_bsize, SWAP_FILE_SIZE_64M/fsbuf.f_bsize);
		}
		else
			return -ENOSPC;
	}
	else
		return -ECANCELED;

	if(create_flag) {
		usb_dbg("%s\n", command);
		if(system(command)) {
			usb_dbg("%s\n", strerror(errno));
			return -1;
		}
	}
	memset(command, 0, PATH_MAX);
	sprintf(command, "/sbin/mkswap %s", swap_file);
	if(system(command)) {
		usb_dbg("%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int using_swap()
{
	char buf[1024] = {0};
	char *cur = NULL;

	f_read_string("/proc/swaps", buf, 1024);
	cur = strstr(buf, SWAP_FILENAME);
	if(cur)
		return 1;
	else
		return 0;
}

void start_swapfile(char *mountpoint)
{
	char swap_file[PATH_MAX] = {0};

	if(tcapi_get_int("SysInfo_Entry", "swap_enable") != 1)
		return;

	if(!using_swap()) {
		if(!create_swapfile(mountpoint)) {
			sprintf(swap_file, "%s/%s/%s", mountpoint, SWAP_FOLDER, SWAP_FILENAME);
			if(swapon(swap_file, 0))
				usb_dbg("%s\n", strerror(errno));
		}
		else
			usb_dbg("%s\n", strerror(errno));
	}
}

void stop_swapfile(char *mountpoint)
{
	char swap_file[PATH_MAX] = {0};

	if(using_swap()) {
		sprintf(swap_file, "%s/%s/%s", mountpoint, SWAP_FOLDER, SWAP_FILENAME);
		if(swapoff(swap_file))
			usb_dbg("%s\n", strerror(errno));
		else
			usb_dbg("swap off %s\n", swap_file);
	}
}
#endif	//TCSUPPORT_SWAP_FILE

#endif	//RTCONFIG_USB
