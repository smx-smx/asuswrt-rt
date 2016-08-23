# dsl-n55u
Custom asuswrt-based firmware for Asus DSL-N55U

This repository is to keep track of ASUS firmware changes, and maybe in the future apply some changes.

There's no current plan/roadmap to do any development now

## Supporting DSL-N55U-D1 ##

This device is a Wireless Router with built-in Trendchip modem.
```sh
# cat /proc/cpuinfo 
system type		: Ralink RT63365 SOC
processor		: 0
cpu model		: MIPS 34K V5.5
BogoMIPS		: 465.30
wait instruction	: yes
microsecond timers	: yes
tlb_entries		: 64
extra interrupt vector	: yes
hardware watchpoint	: yes
ASEs implemented	: mips16 dsp mt
shadow register sets	: 1
VCED exceptions		: not available
VCEI exceptions		: not available
unaligned accesses	: 126836208

processor		: 1
cpu model		: MIPS 34K V5.5
BogoMIPS		: 349.79
wait instruction	: yes
microsecond timers	: yes
tlb_entries		: 64
extra interrupt vector	: yes
hardware watchpoint	: yes
ASEs implemented	: mips16 dsp mt
shadow register sets	: 1
VCED exceptions		: not available
VCEI exceptions		: not available
unaligned accesses	: 126836208

processor		: 2
cpu model		: MIPS 34K V5.5
BogoMIPS		: 349.79
wait instruction	: yes
microsecond timers	: yes
tlb_entries		: 64
extra interrupt vector	: yes
hardware watchpoint	: yes
ASEs implemented	: mips16 dsp mt
shadow register sets	: 1
VCED exceptions		: not available
VCEI exceptions		: not available
unaligned accesses	: 126836208

processor		: 3
cpu model		: MIPS 34K V5.5
BogoMIPS		: 348.97
wait instruction	: yes
microsecond timers	: yes
tlb_entries		: 64
extra interrupt vector	: yes
hardware watchpoint	: yes
ASEs implemented	: mips16 dsp mt
shadow register sets	: 1
VCED exceptions		: not available
VCEI exceptions		: not available
unaligned accesses	: 126836208

# cat /proc/meminfo 
MemTotal:       126400 kB
MemFree:         73588 kB
Buffers:          5708 kB
Cached:          17564 kB
SwapCached:          0 kB
Active:          15012 kB
Inactive:        13140 kB
SwapTotal:           0 kB
SwapFree:            0 kB
Dirty:               0 kB
Writeback:           0 kB
AnonPages:        4924 kB
Mapped:           4488 kB
Slab:            11464 kB
SReclaimable:     3544 kB
SUnreclaim:       7920 kB
PageTables:        408 kB
NFS_Unstable:        0 kB
Bounce:              0 kB
CommitLimit:     63200 kB
Committed_AS:    11816 kB
VmallocTotal:  1048308 kB
VmallocUsed:      8804 kB
VmallocChunk:  1039060 kB
IMEM Remains: 4294964780 Bytes

# cat /proc/mtd 
dev:    size   erasesize  name
mtd0: 00020000 00020000 "bootloader"
mtd1: 00020000 00020000 "romfile"
mtd2: 00112726 00020000 "kernel"
mtd3: 021e7000 00020000 "rootfs"
mtd4: 04000000 00020000 "tclinux"
mtd5: 00380000 00020000 "reservearea"
```
This gives a total partitions size of 107714342 bytes, aka 102 MB. Flash chip is supposed to be 128MB chip.<br>
Ram is 124MB (126400kb).

```sh
# lsmod
Module                  Size  Used by
nf_nat_ftp 2976 0 - Live 0xc0062000
nf_conntrack_ftp 8560 1 nf_nat_ftp, Live 0xc034c000
sr_mod 14944 0 - Live 0xc04ae000
cdrom 41792 1 sr_mod, Live 0xc0853000
cdc_mbim 5216 0 - Live 0xc050c000
qmi_wwan 7488 0 - Live 0xc04b8000
cdc_wdm 11824 2 cdc_mbim,qmi_wwan, Live 0xc04b4000
cdc_ncm 12208 1 cdc_mbim, Live 0xc0375000
rndis_host 6336 0 - Live 0xc037c000
cdc_ether 4736 1 rndis_host, Live 0xc0379000
asix 15872 0 - Live 0xc04a9000
usbnet 15696 6 cdc_mbim,qmi_wwan,cdc_ncm,rndis_host,cdc_ether,asix, Live 0xc021b000
usblp 12064 0 - Live 0xc0371000
ohci_hcd 22128 0 - Live 0xc0352000
ehci_hcd 34992 0 - Live 0xc049f000
nls_cp950 99072 0 - Live 0xc04f2000
nls_cp936 121184 0 - Live 0xc0834000
nls_utf8 1472 0 - Live 0xc0219000
ufsd 367520 0 - Live 0xc08ad000 (P)
ext2 69440 0 - Live 0xc04e0000
ext3 137600 1 - Live 0xc0811000
jbd 60944 1 ext3, Live 0xc04d0000
mbcache 6480 2 ext2,ext3, Live 0xc0349000
usb_storage 77616 1 - Live 0xc04bc000
sg 29152 0 - Live 0xc0337000
sd_mod 24848 2 - Live 0xc0341000
scsi_wait_scan 864 0 - Live 0xc0319000
scsi_mod 89472 5 sr_mod,usb_storage,sg,sd_mod,scsi_wait_scan, Live 0xc035a000
usbcore 117968 13 cdc_mbim,qmi_wwan,cdc_wdm,cdc_ncm,rndis_host,cdc_ether,asix,usbnet,usblp,ohci_hcd,ehci_hcd,usb_storage, Live 0xc0481000
nf_nat_rtsp 5744 0 - Live 0xc0311000
nf_conntrack_rtsp 8080 1 nf_nat_rtsp, Live 0xc004e000
ipt_REJECT 3840 0 - Live 0xc020e000
iptable_filter 2304 1 - Live 0xc020c000
hw_nat 59472 0 - Live 0xc0321000 (P)
mldsnooping 6976 0 - Live 0xc00fd000
igmpsnoop 16160 0 - Live 0xc0210000
ebt_ip6 3360 0 - Live 0xc00f7000
ebt_snat 2176 0 - Live 0xc00fb000
ebt_dnat 1984 0 - Live 0xc00f9000
ebt_ip 2688 0 - Live 0xc0084000
ebtable_broute 1696 0 - Live 0xc0082000
ebtable_nat 1952 0 - Live 0xc0060000
ebtable_filter 1920 0 - Live 0xc005a000
ebtables 23456 7 ebt_ip6,ebt_snat,ebt_dnat,ebt_ip,ebtable_broute,ebtable_nat,ebtable_filter, Live 0xc00ee000
act_mirred 4624 0 - Live 0xc0051000
mt7603eap 1475968 1 - Live 0xc06a7000
rt5592ap 1596720 1 - Live 0xc0520000 (P)
tc3162_dmt 957840 0 - Live 0xc0220000 (P)
tc3162l2sar 74560 1 - Live 0xc006e000 (P)
raeth 66288 2 tc3162_dmt,tc3162l2sar, Live 0xc0086000 (P)
tccicmd 93440 5 mt7603eap,rt5592ap,tc3162_dmt,tc3162l2sar,raeth, Live 0xc00d6000 (P)
tcledctrl 27840 5 mt7603eap,rt5592ap,tc3162l2sar,raeth,tccicmd, Live 0xc0066000 (P)
rtl8367b 249328 1 tcledctrl, Live 0xc0098000 (P)
brg_shortcut 6032 0 - Live 0xc0044000 (P)
tcvlantag 14720 0 - Live 0xc0055000
tcportbind 5072 0 - Live 0xc004b000
module_sel 2272 3 rt5592ap,tcvlantag,tcportbind, Live 0xc0049000 (P)
tcfullcone 3120 0 - Live 0xc0047000
```
