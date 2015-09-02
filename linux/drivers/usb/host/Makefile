#
# Makefile for USB Host Controller Drivers
#

ifeq ($(CONFIG_USB_DEBUG),y)
	EXTRA_CFLAGS		+= -DDEBUG
endif

ifeq ($(CONFIG_USB_SERIAL_OPTION),m)
EXTRA_CFLAGS += -DTC_SUPPORT_3G
endif

ifeq ($(CONFIG_USB_SERIAL_OPTION),y)
EXTRA_CFLAGS += -DTC_SUPPORT_3G
endif

obj-$(CONFIG_PCI)		+= pci-quirks.o

obj-$(CONFIG_USB_EHCI_HCD)	+= ehci-hcd.o
obj-$(CONFIG_USB_ISP116X_HCD)	+= isp116x-hcd.o
obj-$(CONFIG_USB_OHCI_HCD)	+= ohci-hcd.o
obj-$(CONFIG_USB_UHCI_HCD)	+= uhci-hcd.o
obj-$(CONFIG_USB_SL811_HCD)	+= sl811-hcd.o
obj-$(CONFIG_USB_SL811_CS)	+= sl811_cs.o
obj-$(CONFIG_USB_U132_HCD)	+= u132-hcd.o
obj-$(CONFIG_MIPS_RT63365)	+= ehci_ohci.o
