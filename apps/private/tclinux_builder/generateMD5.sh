#! /bin/sh
if [ -f ./$1.bin ];
then
openssl md5 ./$1.bin
fi
if [ -f ./$1.trx ];
then
openssl md5 ./$1.trx
fi
if [ -f ./tcboot.bin ];
then
openssl md5 ./tcboot.bin
fi
