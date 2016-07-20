APPNAME			:= libwrapglLib.so
DESTDIR			?= .
CROSS_COMPILE		= /opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-

ROOTFS			= /home/cylee/rootfs_normal

# Make command to use for dependencies
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar

TARGET_PATH_LIB = $(ROOTFS)/usr/lib
TARGET_PATH_INCLUDE = $(ROOTFS)/usr/include

BIN_TUT = $(DESTDIR)/$(APPNAME)

CFLAGS = -DDEBUG -D_DEBUG -D_GNU_SOURCE -mfloat-abi=softfp -mfpu=neon -fPIC -O3 -fno-strict-aliasing -fno-optimize-sibling-calls -Wall -g

CFLAGS_TUT = $(CFLAGS) $(CFLAGS_EGL)

CFLAGS_TUT += \
    -DLINUX \
	-DEGL_API_FB \

CFLAGS_TUT += \
	-I$(TARGET_PATH_INCLUDE)

OBJECTS_TUT += \
	wrapLib.o \
	fbwrap.o \
	eglwrap.o \

DEPS_TUT = -shared -lstdc++ -lm -lGLESv2 -lEGL -ldl -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB)

$(BIN_TUT) : $(OBJECTS_TUT)
	@echo " LD " $@
	$(QUIET)$(CC) -o $(BIN_TUT) $(OBJECTS_TUT) $(DEPS_TUT)

%.o : %.c
	@echo " CC " $@
	$(QUIET)$(CC) $(CFLAGS_TUT) -c $< -o $@

clean:
	rm -f $(OBJECTS_TUT) $(BIN_TUT)
	rm -f ../../$(APPNAME)
	
install:
	cp -f $(APPNAME) ../../
#	sudo cp -f $(APPNAME) $(TARGET_PATH_LIB)/
#	sudo cp -f *.h $(TARGET_PATH_INCLUDE)/wrapLib/
#	cp -f $(APPNAME) $(DESTDIR)/.
	
	
