#
# Makefile for MIPS-specific library files..
#

ifndef CONFIG_MIPS_PATENTFREE
lib-y	+= csum_partial.o memcpy.o memcpy-inatomic.o memset.o strlen_user.o \
	   strncpy_user.o strnlen_user.o uncached.o
else
lib-y	+= csum_partial_NP.o memcpy_NP.o memcpy-inatomic_NP.o memset_NP.o strlen_user.o \
	   strncpy_user.o strnlen_user.o uncached.o csum_partial_copy.o
endif

obj-y			+= iomap.o
obj-$(CONFIG_PCI)	+= iomap-pci.o

# libgcc-style stuff needed in the kernel
obj-y += ashldi3.o ashrdi3.o lshrdi3.o ucmpdi2.o
