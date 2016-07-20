APPNAME		:= comm_read
CROSS_COMPILE	:= /opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-

CC = $(CROSS_COMPILE)gcc
CFLAGS = -DDEBUG -D_DEBUG -D_GNU_SOURCE -fPIC -O3 -fno-strict-aliasing -fno-optimize-sibling-calls -Wall -g
OBJECTS += comm_read.o

$(APPNAME) : $(OBJECTS)
	@echo " LD " $@
	$(CC) -o $(APPNAME) $(OBJECTS)

%.o : %.c
	@echo " CC " $@
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(APPNAME)
