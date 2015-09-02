# $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/arch/cris/kernel/Makefile#1 $
#
# Makefile for the linux kernel.
#

extra-y	:= vmlinux.lds

obj-y   := process.o traps.o irq.o ptrace.o setup.o \
	   time.o sys_cris.o semaphore.o

obj-$(CONFIG_MODULES)    += crisksyms.o
obj-$(CONFIG_MODULES)	 += module.o
obj-$(CONFIG_SYSTEM_PROFILER)   += profile.o

clean:

