#!/bin/bash
romfile=romfile.cfg

#Checking the files' names
test ! -e $romfile && echo "romfile not exist or wrong name" && exit 0

#because we need create ctromfile.cfg and put it in rootfs first ---added by brian
size_f=$(stat -c%s "$romfile")
echo "../../../tools/trx/trx -d $size_f -f romfile.cfg -o ctromfile.cfg"
echo `../../../tools/trx/trx -d $size_f -f romfile.cfg -o ctromfile.cfg`
echo "cp ctromfile.cfg ../../../filesystem/userfs"
cp ctromfile.cfg ../../../filesystem/userfs

