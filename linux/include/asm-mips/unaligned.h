/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1999, 2000, 2001, 2003 by Ralf Baechle
 * Copyright (C) 1999, 2000, 2001 Silicon Graphics, Inc.
 */
#ifndef _ASM_UNALIGNED_H
#define _ASM_UNALIGNED_H

#ifndef CONFIG_MIPS_PATENTFREE

#include <asm-generic/unaligned.h>

#else /* CONFIG_MIPS_PATENTFREE */

#include <linux/string.h>

/* Use memmove here, so gcc does not insert a __builtin_memcpy. */

#define get_unaligned(ptr) \
  ({ __typeof__(*(ptr)) __tmp; memmove(&__tmp, (ptr), sizeof(*(ptr))); __tmp; })

#define put_unaligned(val, ptr)				\
  ({ __typeof__(*(ptr)) __tmp = (val);			\
     memmove((ptr), &__tmp, sizeof(*(ptr)));		\
     (void)0; })


#endif /* CONFIG_MIPS_PATENTFREE */
#endif /* _ASM_UNALIGNED_H */
