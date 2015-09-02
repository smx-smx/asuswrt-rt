#ifndef __ASM_TC3162_IRQ_H
#define __ASM_TC3162_IRQ_H

#ifdef CONFIG_MIPS_TC3262
#define NR_IRQS 64
#else
#define NR_IRQS 32
#endif

#include_next <irq.h>

#endif /* __ASM_TC3162_IRQ_H */
