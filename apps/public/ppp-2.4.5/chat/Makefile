#	$Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/ppp-2.4.5/chat/Makefile#1 $

DESTDIR = $(INSTROOT)/usr/local
BINDIR = $(DESTDIR)/sbin
MANDIR = $(DESTDIR)/share/man/man8

CDEF1=	-DTERMIOS			# Use the termios structure
CDEF2=	-DSIGTYPE=void			# Standard definition
CDEF3=	-UNO_SLEEP			# Use the usleep function
CDEF4=	-DFNDELAY=O_NDELAY		# Old name value
CDEFS=	$(CDEF1) $(CDEF2) $(CDEF3) $(CDEF4)

COPTS=	-O2 -g -pipe
CFLAGS=	$(COPTS) $(CDEFS)

INSTALL= install

all:	chat

chat:	chat.o
	$(CC) -o chat chat.o
	$(STRIP) --strip-unneeded chat
chat.o:	chat.c
	$(CC) -c $(CFLAGS) -o chat.o chat.c

install: chat
	mkdir -p $(BINDIR) $(MANDIR)
	$(INSTALL) -s -c chat $(BINDIR)
	$(INSTALL) -c -m 644 chat.8 $(MANDIR)

clean:
	rm -f chat.o chat *~
