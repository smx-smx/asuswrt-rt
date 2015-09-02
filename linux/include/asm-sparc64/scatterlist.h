/* $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/include/asm-sparc64/scatterlist.h#1 $ */
#ifndef _SPARC64_SCATTERLIST_H
#define _SPARC64_SCATTERLIST_H

#include <asm/page.h>
#include <asm/types.h>

struct scatterlist {
	struct page	*page;
	unsigned int	offset;

	unsigned int	length;

	dma_addr_t	dma_address;
	__u32		dma_length;
};

#define sg_dma_address(sg)	((sg)->dma_address)
#define sg_dma_len(sg)     	((sg)->dma_length)

#define ISA_DMA_THRESHOLD	(~0UL)

#endif /* !(_SPARC64_SCATTERLIST_H) */
