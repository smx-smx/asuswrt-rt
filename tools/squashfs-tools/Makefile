INCLUDEDIR = .
LZMAPATH = ./lzma/C/7zip/Compress/LZMA_Lib

CFLAGS := -I$(INCLUDEDIR) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -O2
CXX = g++ -O3 -Wall
AR = ar
RM = rm -f
CC = gcc


all: mksquashfs mksquashfs-lzma unsquashfs

mksquashfs: mksquashfs.o read_fs.o sort.o
	$(CC) mksquashfs.o read_fs.o sort.o -lz -o $@

mksquashfs-lzma: mksquashfs.o read_fs.o sort.o
	$(CXX) mksquashfs.o read_fs.o sort.o -L$(LZMAPATH) -llzma -o $@

mksquashfs.o: mksquashfs.c squashfs_fs.h mksquashfs.h global.h sort.h

read_fs.o: read_fs.c squashfs_fs.h read_fs.h global.h

sort.o: sort.c squashfs_fs.h global.h sort.h

unsquashfs: unsquashfs.o
	$(CC) unsquashfs.o -lz -o $@

unsquashfs.o: unsquashfs.c squashfs_fs.h read_fs.h global.h

clean:
	rm -f mksquashfs mksquashfs-lzma unsquashfs *.o
