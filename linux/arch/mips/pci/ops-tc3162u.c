#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>

uint32 pcie_config_addr = 0x1fb81cf8;
uint32 pcie_config_data = 0x1fb81cfc;

#undef DEBUG

//#define DEBUG
#ifdef DEBUG
#define DBG(f, a...)	printk(f, ## a )
#else
#define DBG(f, a...)	do {} while (0)
#endif

#define PCIE_ENABLE 0x80000000

static spinlock_t pcie_lock = SPIN_LOCK_UNLOCKED;

/* -------------------------------------------------------------------------*/

static inline void write_cfgaddr(u32 addr)
{
	__raw_writel(addr,
			(void __iomem *)(KSEG1ADDR(pcie_config_addr)));
//	__raw_writel((addr | PCI_ENABLE),
//		(void __iomem *)(KSEG1ADDR(PCI_CONFIG_ADDR)));
}

static inline void write_cfgdata(u32 data)
{
	__raw_writel(data, (void __iomem *)KSEG1ADDR(pcie_config_data));
}

static inline u32 read_cfgdata(void)
{
	u32 tmp;
	if(isRT63365)
		 tmp = __raw_readl((void __iomem *)KSEG1ADDR(pcie_config_data));
	return __raw_readl((void __iomem *)KSEG1ADDR(pcie_config_data));
}

static inline u32 mkaddr(struct pci_bus *bus, unsigned int devfn, int where)
{
	u32 type;

	if(isRT63165 || isRT63365){
		return	(((bus->number & 0xFF) << 24) | ((devfn & 0xFF) << 16) |\
				(where & 0xFFC));
	}else{
		type=(bus->number & 0xFF)?PCIE_ENABLE:0;

		return (type | ((bus->number & 0xFF) << 20) | ((devfn & 0xFF) << 12) | \
			(where & 0xFFC));
	}
}

/* -------------------------------------------------------------------------*/

static int tc3162_pciebios_read(struct pci_bus *bus, unsigned int devfn, int where,
		int size, u32 *val)
{
	unsigned long flags;
	u32 data;

	spin_lock_irqsave(&pcie_lock, flags);
	write_cfgaddr(mkaddr(bus,devfn,where));
	data = read_cfgdata();

	DBG("PCIE: cfg_read  %02u.%02u.%01u/%02X:%01d, cfg:0x%08X",
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

	spin_unlock_irqrestore(&pcie_lock, flags);

	return PCIBIOS_SUCCESSFUL;
}

static int tc3162_pciebios_write(struct pci_bus *bus, unsigned int devfn, int where,
		int size, u32 val)
{
	unsigned long flags;
	u32 data;
	int s;

	spin_lock_irqsave(&pcie_lock, flags);

	write_cfgaddr(mkaddr(bus,devfn,where));
	data = read_cfgdata();

	DBG("PCIE: cfg_write %02u.%02u.%01u/%02X:%01d,%08x, cfg:0x%08X",
		bus->number, PCI_SLOT(devfn), PCI_FUNC(devfn),
		where, size, val, data);

	switch (size) {
	case 1:
		s = ((where & 3) << 3);
		data &= ~(0xFF << s);
		data |= ((val & 0xFF) << s);
		break;
	case 2:
		//s = ((where & 2) << 4);
		s = ((where & 2) << 3);//krammer try
		data &= ~(0xFFFF << s);
		data |= ((val & 0xFFFF) << s);
		break;
	case 4:
		data = val;
		break;
	}

	write_cfgdata(data);
	DBG(", 0x%08X written\n", data);

	spin_unlock_irqrestore(&pcie_lock, flags);

	return PCIBIOS_SUCCESSFUL;
}

struct pci_ops tc3162_pcie_ops = {
    .read   = tc3162_pciebios_read,
    .write  = tc3162_pciebios_write
};
