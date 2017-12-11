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
rm -rf *xa*
test -e ./padding_k && rm -rf ./padding_k
test -e ./padding_r && rm -rf ./padding_r
test -e ./tclinux

#check FW size
echo -n "Check firmware size..."
FW_SIZE=`stat -c%s tclinux.bin`
if [ "$FW_SIZE" -gt "$FW_SIZE_LIMIT" ]; then
	echo "Oversize!!! ($FW_SIZE > $FW_SIZE_LIMIT)";
	exit 1;
else
	echo "Pass"
fi
