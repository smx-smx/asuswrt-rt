#! /bin/sh
openssl md5 ./$1_$2.bin
openssl md5 ./$1_$2.trx
openssl md5 ./tcboot.bin
