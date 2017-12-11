#!/bin/bash
test -e ./tclinux_allineone && rm -rf tclinux_allineone
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

#replace ATE test config value
sed -i 's/HT_BW="1"/HT_BW="2"/g' romfile.cfg
sed -i 's/wl0_HT_BW="1"/wl0_HT_BW="2"/g' romfile.cfg
sed -i 's/wl1_HT_BW="1"/wl1_HT_BW="2"/g' romfile.cfg
sed -i 's/telnetd_enable="0"/telnetd_enable="1"/g' romfile.cfg

#Calculating the files' sizes
size_b=$(stat -c%s "$blder")
size_f=$(stat -c%s "$romfile")
size_k=$(stat -c%s "$kernel")
size_r=$(stat -c%s "$rootfs")

echo "size b f k r"$size_b $size_f $size_k $size_r

#Calculating the size of the bootloader paddings
if [ "$TCSUPPORT_BOOTROM_LARGE_SIZE" != "" ] && [ "$TCSUPPORT_BB_NAND" != "" ];then
	let "remainder_b=131072-$size_b"
else
	let "remainder_b=65536-$size_b"
fi

if [ "$TCSUPPORT_2_6_36_KERNEL" != "" ] && [ "$TCSUPPORT_BOOTROM_LARGE_SIZE" != "" ] && [ "$TCSUPPORT_BB_NAND" != "" ];then
	#Generating tclinux_allinone for NAND,tcboot is 256k,romfile is 256k
	let "remainder_b=262144-$size_b"
	let "remainder_f=262144-$size_f"
	echo "Need a padding for NAND bootloader" "remainder_b " $remainder_b "remainder_f " $remainder_f
	split -b $remainder_b padding
	mv xaa padding_b
	echo "Need a padding for NAND romfile"
	split -b $remainder_f padding
	mv xaa padding_f
	test ! -e ./tclinux.bin && echo "tclinux.bin is not exist."		
	cat tcboot.bin padding_b romfile.cfg padding_f tclinux.bin > tclinux_allinone
	if [ "$TCSUPPORT_CT" == "" ] ;then
		echo "add crc32 at the end of tclinux_allineone"
		../../../tools/trx/trx  -g
	fi
	echo `./ecc_generator tclinux_allinone tclinux_allinone_ecc`
	rm -f tclinux_allinone
	mv tclinux_allinone_ecc tclinux_allinone
else
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

if [ "$TCSUPPORT_CT" == "" ] ;then
	echo "add crc32 at the end of tclinux_allineone"
	../../../tools/trx/trx  -g
fi

fi

rm -f *xa*
test -e ./padding_b && rm -f ./padding_b
test -e ./padding_f && rm -f ./padding_f
test -e ./tclinux && rm -f ./tclinux
