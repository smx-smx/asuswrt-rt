#!/bin/bash
test -e ./tclinux && rm -rf *tclinux*
blder=tcboot.bin
romfile=romfile.cfg
if [ "$TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT" != "" ] ;then
romfile=ctromfile.cfg
else
romfile=romfile.cfg	
fi
kernel=linux.7z
rootfs=rootfs
TOP_DIR=`pwd`

if [ "$TCSUPPORT_RESTORE_ROM_T" != "" ] ;then
../../../tools/restore_rom_t_info/restore_rom_t_info
fi
#Checking the files' names
test ! -e $blder && echo "bootloader not exist or wrong name" && exit 0
test ! -e $romfile && echo "romfile not exist or wrong name" && exit 0
test ! -e $kernel && echo "linux.7z not exist or wrong name" && exit 0
test ! -e $rootfs && echo "rootfs not exist or wrong name" && exit 0

#Calculating the files' sizes
size_b=$(stat -c%s "$blder")
size_f=$(stat -c%s "$romfile")
size_k=$(stat -c%s "$kernel")
size_r=$(stat -c%s "$rootfs")
#if [ "$1" = "pb" ] 
#then
#	upperBound=655360
#else
#	upperBound=851968
#fi

#Calculating the size of the bootloader paddings
if [ "$TCSUPPORT_BOOTROM_LARGE_SIZE" != "" ] && [ "$TCSUPPORT_BB_NAND" != "" ];then
	let "remainder_b=131072-$size_b"
else
	let "remainder_b=65536-$size_b"
fi

#Calculating the size of the romfile paddings
let "remainder_f=65536-$size_f"

if [ "$remainder_b" -eq 0 ]
then
	echo "Need NO padding for bootloader"
else
	echo "Need a padding for bootloader"
	split -b $remainder_b padding
	mv xaa padding_b
fi

if [ "$remainder_f" -eq 0 ]
then
	echo "Need NO padding for romfile"
else
	echo "Need a padding for romfile"
	split -b $remainder_f padding
	mv xaa padding_f
fi

#Claculating the size of the kernel paddings
#if [ "$2" = "tclinux" ]
#then
#	let "remainder_k=$upperBound-$size_k"
#else
#	let "remainder_k=0"
#fi

#Claculating the size of the rootfs paddings
#if [ "$2" = "tclinux" ]
#then
#let "remainder_r=$size_r%0x10000"
#else
#let "remainder_r=0"
#fi
#let "remainder_k=0"
let "remainder_k=0"
let "remainder_r=0"
if [ "$remainder_k" -eq 0 ]
then
	echo "Need NO padding for kernel"
else
	echo "Need a padding for kernel"
	#Preparing the kernel padding for 64k allignment
	split -b $remainder_k padding
	mv xaa padding_k
fi

if [ "$remainder_r" -eq 0 ]
then
	echo "Need NO padding for rootfs"
else
	echo "Need a padding for rootfs"
	#Preparing the rootfs padding for 64k allignment
	let "size_rp=($size_r/0x10000+1)*0x10000-$size_r"
	split -b $size_rp padding
	mv xaa padding_r
fi

#Generating tclinux.bin with trx header
if [ "$remainder_k" -eq 0 ] && [ "$remainder_r" -eq 0 ]
then
	cat linux.7z rootfs > tclinux
elif [ "$remainder_k" -eq 0 ] && [ "$remainder_r" -ne 0 ]
then
	cat linux.7z rootfs padding_r > tclinux
elif [ "$remainder_k" -ne 0 ] && [ "$remainder_r" -eq 0 ]
then
	cat linux.7z padding_k rootfs > tclinux
else
	cat linux.7z padding_k rootfs padding_r > tclinux
fi

#if [ "$2" != "tclinux" ]
#then
#	echo "../../tools/trx/trx -k $size_k -f  tclinux -o tclinux.bin"
#	echo `../../tools/trx/trx -k $size_k -f tclinux -o tclinux.bin`
if [ "$TCSUPPORT_2_6_36_KERNEL" != "" ] ;then
if [ "$TCSUPPORT_FREE_BOOTBASE" != "" ] ;then
	echo "../../tools/trx/trx -k $size_k -r $size_r -u $START_ADDR -f tclinux -o tclinux.bin -c ../../tools/trx/trx_config"
	echo `../../../tools/trx/trx -k $size_k -r $size_r -u $START_ADDR -f tclinux -o tclinux.bin -c ../../../tools/trx/trx_config`
else
	echo "../../tools/trx/trx -k $size_k -r $size_r -f tclinux -o tclinux.bin -c ../../tools/trx/trx_config"
	echo `../../../tools/trx/trx -k $size_k -r $size_r -f tclinux -o tclinux.bin -c ../../../tools/trx/trx_config`
fi
else
	echo "../../tools/trx/trx -k $size_k -r $size_r -f tclinux -o tclinux.bin -c ../../tools/trx/trx_config"
	echo `../../../tools/trx/trx -k $size_k -r $size_r -f tclinux -o tclinux.bin -c ../../../tools/trx/trx_config`
fi
#fi
rm -rf *xa*
test -e ./padding_k && rm -rf ./padding_k
test -e ./padding_r && rm -rf ./padding_r
#test -e ./tclinux && rm -rf ./tclinux
test -e ./tclinux
#if [ "$2" != "tclinux" ]
#then
	echo "output file: tclinux.bin"
#else
#	echo "output file: $2"
#fi
#cp ras ~/src/.1

if [ "$TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT" == "" ] ;then
#Generating tclinux_allinone & tclinux_allinone.swap
if [ "$remainder_b" -eq 0 ] && [ "$remainder_f" -eq 0 ]
then
	cat tcboot.bin romfile.cfg tclinux.bin > tclinux_allinone
elif [ "$remainder_b" -eq 0 ] && [ "$remainder_f" -ne 0 ]
then
	cat tcboot.bin romfile.cfg padding_f tclinux.bin > tclinux_allinone
elif [ "$remainder_b" -ne 0 ] && [ "$remainder_f" -eq 0 ]
then
	cat tcboot.bin padding_b romfile.cfg tclinux.bin > tclinux_allinone
else
	cat tcboot.bin padding_b romfile.cfg padding_f tclinux.bin > tclinux_allinone
fi

else
if [ "$remainder_b" -eq 0 ] && [ "$remainder_f" -eq 0 ]
then
	cat tcboot.bin ctromfile.cfg tclinux.bin > tclinux_allinone
elif [ "$remainder_b" -eq 0 ] && [ "$remainder_f" -ne 0 ]
then
	cat tcboot.bin ctromfile.cfg padding_f tclinux.bin > tclinux_allinone
elif [ "$remainder_b" -ne 0 ] && [ "$remainder_f" -eq 0 ]
then
	cat tcboot.bin padding_b ctromfile.cfg tclinux.bin > tclinux_allinone
else
	cat tcboot.bin padding_b ctromfile.cfg padding_f tclinux.bin > tclinux_allinone
fi
fi

if [ "$TCSUPPORT_2_6_36_KERNEL" != "" ] ;then
if [ "$TCSUPPORT_BOOTROM_LARGE_SIZE" != "" ] && [ "$TCSUPPORT_BB_NAND" != "" ];then
#Generating tclinux_allinone for NAND,tcboot is 256k,romfile is 256k
	let "remainder_b=262144-$size_b"
	let "remainder_f=262144-$size_f"
	echo "Need a padding for NAND bootloader"
	split -b $remainder_b padding
	mv xaa padding_b
	echo "Need a padding for NAND romfile"
	split -b $remainder_f padding
	mv xaa padding_f
	cat tcboot.bin padding_b romfile.cfg padding_f tclinux.bin > tclinux_allinone_nand
	#echo `./byteswap tclinux_allinone_nand`
fi
fi

if [ "$TCSUPPORT_CT" == "" ] ;then
#add crc32 at the end of tclinux_allineone
../../../tools/trx/trx  -g
fi

sed -i 's/HT_BW="1"/HT_BW="2"/g' tclinux_allinone
sed -i 's/wl0_HT_BW="1"/wl0_HT_BW="2"/g' tclinux_allinone
sed -i 's/wl1_HT_BW="1"/wl1_HT_BW="2"/g' tclinux_allinone
sed -i 's/telnetd_enable="0"/telnetd_enable="1"/g' tclinux_allinone

#echo `./byteswap tclinux_allinone`

rm -f *xa*
test -e ./padding_b && rm -f ./padding_b
test -e ./padding_f && rm -f ./padding_f
test -e ./tclinux && rm -f ./tclinux
