# $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/arch/sparc64/mm/Makefile#1 $
# Makefile for the linux Sparc64-specific parts of the memory manager.
#

EXTRA_AFLAGS := -ansi
EXTRA_CFLAGS := -Werror

obj-y    := ultra.o tlb.o tsb.o fault.o init.o generic.o

obj-$(CONFIG_HUGETLB_PAGE) += hugetlbpage.o
