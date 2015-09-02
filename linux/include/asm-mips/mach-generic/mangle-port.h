/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003, 2004 Ralf Baechle
 */
#ifndef __ASM_MACH_GENERIC_MANGLE_PORT_H
#define __ASM_MACH_GENERIC_MANGLE_PORT_H

#if defined(CONFIG_MIPS_TC3262)
extern unsigned char io_swap_noneed;
#endif

#define __swizzle_addr_b(port)	(port)
#define __swizzle_addr_w(port)	(port)
#define __swizzle_addr_l(port)	(port)
#define __swizzle_addr_q(port)	(port)

/*
 * Sane hardware offers swapping of PCI/ISA I/O space accesses in hardware;
 * less sane hardware forces software to fiddle with this...
 *
 * Regardless, if the host bus endianness mismatches that of PCI/ISA, then
 * you can't have the numerical value of data and byte addresses within
 * multibyte quantities both preserved at the same time.  Hence two
 * variations of functions: non-prefixed ones that preserve the value
 * and prefixed ones that preserve byte addresses.  The latters are
 * typically used for moving raw data between a peripheral and memory (cf.
 * string I/O functions), hence the "__mem_" prefix.
 */
#if defined(CONFIG_SWAP_IO_SPACE)

# define ioswabb(a,x)		(x)
# define __mem_ioswabb(a,x)	(x)
//auto Choose SWAP function according to platform;
#if defined(CONFIG_MIPS_TC3262)
# define ioswabw(a,x)       (io_swap_noneed ? (x) : le16_to_cpu(x) )
# define __mem_ioswabw(a,x) (io_swap_noneed ? cpu_to_le16(x) : (x) )
# define ioswabl(a,x)		(io_swap_noneed ? (x) : le32_to_cpu(x) )
# define __mem_ioswabl(a,x)	(io_swap_noneed ? cpu_to_le32(x) : (x) )
# define ioswabq(a,x)		(io_swap_noneed ? (x) : le64_to_cpu(x) )
# define __mem_ioswabq(a,x) (io_swap_noneed ? cpu_to_le32(x) : (x) )
#else 
# define ioswabw(a,x)		le16_to_cpu(x)
# define __mem_ioswabw(a,x)	(x)
# define ioswabl(a,x)		le32_to_cpu(x)
# define __mem_ioswabl(a,x)	(x)
# define ioswabq(a,x)		le64_to_cpu(x)
# define __mem_ioswabq(a,x)	(x)

#endif
#else

# define ioswabb(a,x)		(x)
# define __mem_ioswabb(a,x)	(x)
# define ioswabw(a,x)		(x)
# define __mem_ioswabw(a,x)	cpu_to_le16(x)
# define ioswabl(a,x)		(x)
# define __mem_ioswabl(a,x)	cpu_to_le32(x)
# define ioswabq(a,x)		(x)
# define __mem_ioswabq(a,x)	cpu_to_le32(x)

#endif

#endif /* __ASM_MACH_GENERIC_MANGLE_PORT_H */
