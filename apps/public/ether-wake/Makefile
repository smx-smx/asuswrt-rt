#***********************************************************************
#
# Makefile
#
#***********************************************************************
all: ether-wake


ether-wake: ether-wake.o
	$(CC) $(CFLAGS) $(LIBS)  $^ -o $@


.PHONY: clean
clean:
	-rm -rf *.o ether-wake

