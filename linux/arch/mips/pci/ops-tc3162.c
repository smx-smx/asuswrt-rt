#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>

#define PCI_CONFIG_ADDR		0x1fb80cf8
#define PCI_CONFIG_DATA		0x1fb80cfc

#undef DEBUG

#ifdef DEBUG
#define DBG(f, a...)	printk(f, ## a )
#else
#define DBG(f, a...)	do {} while (0)
#endif

#define PCI_ENABLE 0x80000000

static spinlock_t pci_lock = SPIN_LOCK_UNLOCKED;

/* -------------------------------------------------------------------------*/

static inline void write_cfgaddr(u32 addr)
{
	__raw_writel((addr | PCI_ENABLE),
		(void __iomem *)(KSEG1ADDR(PCI_CONFIG_ADDR)));
}

static inline void write_cfgdata(u32 data)
{
	__raw_writel(data, (void __iomem *)KSEG1ADDR(PCI_CONFIG_DATA));
}

static inline u32 read_cfgdata(void)
{
	return __raw_readl((void __iomem *)KSEG1ADDR(PCI_CONFIG_DATA));
}

static inline u32 mkaddr(struct pci_bus *bus, unsigned int devfn, int where)
{
#ifndef PCIE_PCI_COEXIT
	return (((bus->number & 0xFF) << 16) | ((devfn & 0xFF) << 8) | \
		(where & 0xFC));
#else
	return (((bus->number & 0) << 16) | ((devfn & 0xFF) << 8) | \
		(where & 0xFC));
#endif
}

/* -------------------------------------------------------------------------*/

static int tc3162_pcibios_read(struct pci_bus *bus, unsigned int devfn, int where,
		int size, u32 *val)
{
	unsigned long flags;
	u32 data;

	spin_lock_irqsave(&pci_lock, flags);

	write_cfgaddr(mkaddr(bus,devfn,where));
	data = read_cfgdata();

	DBG("PCI: cfg_read  %02u.%02u.%01u/%02X:%01d, cfg:0x%08X",
		bus->number, PCI_SLOT(devfn), PCI_FUNC(devfn),
		where, size, data);

	switch (size) {
	case 1:
		if (where & 1)
			data >>= 8;
		if (where & 2)
			data >>= 16;
		data &= 0xFF;
		break;
	case 2:
		if (where & 2)
			data >>= 16;
		data &= 0xFFFF;
		break;
	}

	*val = data;
	DBG(", 0x%08X returned\n", data);

	spin_unlock_irqrestore(&pci_lock, flags);

	return PCIBIOS_SUCCESSFUL;
}

static int tc3162_pcibios_write(struct pci_bus *bus, unsigned int devfn, int where,
		int size, u32 val)
{
	unsigned long flags;
	u32 data;
	int s;

	spin_lock_irqsave(&pci_lock, flags);

	write_cfgaddr(mkaddr(bus,devfn,where));
	data = read_cfgdata();

	DBG("PCI: cfg_write %02u.%02u.%01u/%02X:%01d, cfg:0x%08X",
		bus->number, PCI_SLOT(devfn), PCI_FUNC(devfn),
		where, size, data);

	switch (size) {
	case 1:
		s = ((where & 3) << 3);
		data &= ~(0xFF << s);
		data |= ((val & 0xFF) << s);
		break;
	case 2:
		s = ((where & 2) << 4);
		data &= ~(0xFFFF << s);
		data |= ((val & 0xFFFF) << s);
		break;
	case 4:
		data = val;
		break;
	}

	write_cfgdata(data);
	DBG(", 0x%08X written\n", data);

	spin_unlock_irqrestore(&pci_lock, flags);

	return PCIBIOS_SUCCESSFUL;
}

struct pci_ops tc3162_pci_ops = {
    .read   = tc3162_pcibios_read,
    .write  = tc3162_pcibios_write
};
