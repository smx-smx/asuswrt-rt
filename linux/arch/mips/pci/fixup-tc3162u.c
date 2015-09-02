#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <asm/tc3162/tc3162.h>

#ifndef PCIE_PCI_COEXIT
/*
static char irq_tab_tc3162_pcie[] __initdata = {
    //[1] = PCI_A_INT,
    //[2] = PCI_B_INT,
    [0] = PCIE_A_INT,//krammer
};
*/
int __init pcibios_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	//if (slot <= 0)
#if 0
	if (slot < 0)
		return -1;
#endif
    	//return irq_tab_tc3162[slot];
	return PCIE_A_INT;
}
#endif

extern int dual_band_support;
static void tc3162_pcie_fixup(struct pci_dev *dev)
{
	/* setup COMMAND register */
	pci_write_config_word(dev, PCI_COMMAND,
		(PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));

	/* setup CACHE_LINE_SIZE register */
	pci_write_config_byte(dev, PCI_CACHE_LINE_SIZE, 0x8);

	/* setup LATENCY_TIMER register */
	pci_write_config_byte(dev, PCI_LATENCY_TIMER, 0x20);

	/* setup BARS */
//	pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0);
//	pci_write_config_dword(dev, PCI_BASE_ADDRESS_1, 0x1FBA0000);
//	pci_write_config_dword(dev, PCI_BASE_ADDRESS_2, 0x1FBB0000);
}
static void tc3162_pcie_fixup_ra63165(struct pci_dev *dev)
{
	/* setup COMMAND register */
	pci_write_config_word(dev, PCI_COMMAND,
		(PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));
		
	//pci-e interrupt enable_dma
	if(isRT63365){
		if(VPint(0xbfb82050) == 1){
		VPint(0xbfb8000c) |= (1<<20); 
		}
		//second band
		if(dual_band_support){
		if(VPint(0xbfb83050) == 1){
			VPint(0xbfb8000c) |= (1<<21);
			}
		}
	}else{
		VPint(0xbfb8100c) |= (1<<20);
	}	

}
#ifndef PCIE_PCI_COEXIT
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}
#endif

DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_SIS, PCI_DEVICE_ID_SIS,
          tc3162_pcie_fixup);
DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_RT, PCI_DEVICE_ID_RT,
          tc3162_pcie_fixup_ra63165);	  

