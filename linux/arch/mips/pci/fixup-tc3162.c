#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <asm/tc3162/tc3162.h>

static char irq_tab_tc3162[] __initdata = {
#ifndef CONFIG_MIPS_TC3262
    [1] = PCI_A_INT,
    [2] = PCI_B_INT,
#endif
};

int __init pcibios_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
#ifndef PCIE_PCI_COEXIT
	if (slot <= 0)
		return -1;
#else
	if (slot == 0){
		return PCIE_A_INT;
	}
#ifdef CONFIG_MIPS_TC3262	
	else if((isRT63365) && (slot == 1)){
		return USB11_INT; //rt63365 use original usb 11 INT number
	}
#endif	
#endif
    return irq_tab_tc3162[slot];
}

static void tc3162_pci_fixup(struct pci_dev *dev)
{
	/* setup COMMAND register */
	pci_write_config_word(dev, PCI_COMMAND,
		(PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));

	/* setup CACHE_LINE_SIZE register */
	pci_write_config_byte(dev, PCI_CACHE_LINE_SIZE, 0x8);

	/* setup LATENCY_TIMER register */
	pci_write_config_byte(dev, PCI_LATENCY_TIMER, 0x20);

	/* setup BARS */
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_1, 0x1FBA0000);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_2, 0x1FBB0000);
}

int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}

DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_TRENDCHIP, PCI_DEVICE_ID_TC3162,
          tc3162_pci_fixup);

