APPNAME			:= SDCard_bench
TIIAPPNAME			:= TZSDCard_bench
DESTDIR			?= .
CROSS_COMPILE		:= /opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-
ROOTFS			:= /home/swkim/rootfs_normal
# Make command to use for dependencies
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar


TARGET_PATH_LIB = $(ROOTFS)/usr/lib
TARGET_PATH_INCLUDE = $(ROOTFS)/usr/include
#COMMON_DIR=../common

BIN_TUT = $(DESTDIR)/$(APPNAME)

BIN_TUT_TII = $(DESTDIR)/$(TIIAPPNAME)

CFLAGS = -DDEBUG -D_DEBUG -D_GNU_SOURCE -mfloat-abi=softfp -mfpu=neon -fPIC -O3 -fno-strict-aliasing -fno-optimize-sibling-calls -Wall -g

CFLAGS_TII = -DSHARED_DEVICE -DDEBUG -D_DEBUG -D_GNU_SOURCE -mfloat-abi=softfp -mfpu=neon -fPIC -O3 -fno-strict-aliasing -fno-optimize-sibling-calls -Wall -g

CFLAGS_TUT = $(CFLAGS) $(CFLAGS_EGL)

CFLAGS_TUT_TII = $(CFLAGS_TII) $(CFLAGS_EGL)

CFLAGS_TUT += \
    -DLINUX \
	-DEGL_API_FB \

CFLAGS_TUT += \
	-I$(TARGET_PATH_INCLUDE) \
	-I$(COMMON_DIR)/inc \
	

OBJECTS_TUT += \
	sdbench.o
	#$(COMMON_DIR)/src/fsl_egl.o

OBJECTS_TUT_TII += \
	tzsdbench.o \
	wrapLib.o
	#$(COMMON_DIR)/src/fsl_egl.o

DEPS_TUT = -lstdc++ -lm -ldl -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB)

all :
	$(CC) -c $(CFLAGS_TUT_TII) wrapLib.c
	$(CC) -c $(CFLAGS_TUT_TII) sdbench.c
	$(CC) -o TZSDCard_bench wrapLib.o sdbench.o
	$(CC) -c $(CFLAGS_TUT) sdbench.c
	$(CC) -o SDCard_bench sdbench.o
	$(CC) -o reset_tz_log_counter reset_tz_log_counter.c


clean:
	rm -f $(OBJECTS_TUT) $(OBJECTS_TUT:.o=.d) $(BIN_TUT) wrapLib.o reset_tz_log_counter
	rm -f $(OBJECTS_TUT_TII) $(OBJECTS_TUT_TII:.o=.d) $(BIN_TUT_TII)
	cp -f sdbench.c tzsdbench.c 
	rm out_file
	touch out_file

install:
	cp -f $(APPNAME) $(DESTDIR)/.
	
-include $(OBJECTS_TUT:.o=.d)
