# include ../common.mak

CFLAGS = -Os -Wall $(EXTRACFLAGS) -fPIC
#CFLAGS += -I. -I$(TOP)/shared -I$(SRCBASE)/include
CFLAGS += -I. $(TC_CFLAGS)
CFLAGS += -I$(APP_TCAPILIB_DIR)

# ifeq ($(RTCONFIG_SHP),y)
# CFLAGS += -DMEDIA_REVIEW
# endif

# ifeq ($(RTCONFIG_RALINK),y)
# CFLAGS += -I./sysdeps/ralink -I$(TOP)/wireless_tools/ -DWE_NOLIBM 
# endif

# OBJS = shutils.o wl.o wl_linux.o linux_timer.o defaults.o model.o rtstate.o boardapi.o
OBJS = shutils.o model.o rtstate.o
# OBJS += misc.o version.o files.o strings.o process.o 
OBJS += misc.o files.o strings.o process.o
# OBJS += bin_sem_asus.o semaphore.o pids.o discover.o
OBJS += notify_rc.o
OBJS += bin_sem_asus.o semaphore.o pids.o
OBJS += base64.o
OBJS += tcutils.o
OBJS += network_utility.o

# ifeq ($(RTCONFIG_RALINK),y)
# OBJS += $(TOP)/wireless_tools/iwlib.o
# ifeq ($(RTCONFIG_RALINK_RT3052),y)
# OBJS += ra3052.o rtl8367m.o flash_mtd.o api-ralink.o
# else
# ifeq ($(RTCONFIG_DSL),y)
# OBJS += rtl8367r.o flash_mtd.o api-ralink.o
# else
# OBJS += rtl8367m.o flash_mtd.o api-ralink.o
# endif
# endif
# else
# OBJS += broadcom.o api-broadcom.o
# endif

OBJS_CFG_MGR = shutils.o files.o strings.o process.o bin_sem_asus.o semaphore.o pids.o base64.o

ifeq ($(RTCONFIG_USB),y)
OBJS_CFG_MGR += usb.o
OBJS += usb.o
# OBJS += $(TOP)/busybox/util-linux/volume_id/ext.o
# OBJS += $(TOP)/busybox/util-linux/volume_id/util2.o
# OBJS += $(TOP)/busybox/util-linux/volume_id/fat.o
# OBJS += $(TOP)/busybox/util-linux/volume_id/ntfs.o
# OBJS += $(TOP)/busybox/util-linux/volume_id/linux_swap.o
endif

# vpath %.c sysdeps sysdeps/broadcom sysdeps/ralink

all: libshared.so libshared.a

# ifeq ($(CONFIG_BCMWL5),y)
# OBJS += wlif_utils.o UdpLib.o
# endif


libshared.so: $(OBJS)
	# @echo " [shared] LD $@"
	# @$(LD) $(LDFLAGS) -shared -o $@ $^

	# $(SIZECHECK)
	# $(CPTMP)
	$(CC) -shared $(CFLAGS) -o $@ $^
	$(STRIP) $@

libshared.a: $(OBJS)
	@echo " [shared] AR $@"
	@rm -f $@
	@$(AR) cruv $@ $^


# install: all
	# @echo " [shared] Installing as $(INSTALLDIR)/usr/lib/libshared.so"
	# @install -d $(INSTALLDIR)/usr/lib
	# @install -m 755 libshared.so $(INSTALLDIR)/usr/lib
	# @$(STRIP) $(INSTALLDIR)/usr/lib/libshared.so


clean:
	rm -f *.o *.so *.a .*.depend

%.o: %.c .%.depend
	@echo " [shared] CC $@ $(CFLAGS)"
	@$(CC) $(CFLAGS) -o $@ -c $<

.depend: $(OBJS:%.o=%.c)
	@$(CC) $(CFLAGS) -M $^ > .depend

.%.depend: %.c
	@$(CC) $(CFLAGS) -M $< > $@

-include $(OBJS:%.o=.%.depend)

