#!/bin/bash

rm -f *.so

CC="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc"
ROOTFS="/home/cylee/rootfs_secure"

LIB_PATH="$ROOTFS/usr/lib"
INC_PATH="$ROOTFS/usr/include"

FLAG_DEBUG="-DDEBUG -D_DEBUG"
FLAG_LINUX="-DLINUX -DEGL_API_FB"
CFLAGS="-D_GNU_SOURCE -fPIC -fno-strict-aliasing -g $FLAG_DEBUG $FLAG_LINUX"

$CC $CFLAGS -shared -o libhalfdpy.so halfdpy.c -lEGL -lGLESv2 -ldl -L$LIB_PATH -I$INC_PATH
$CC $CFLAGS -shared -o libhalfdpy-surf.so surf_halfdpy.c -lEGL -lGLESv2 -ldl -L$LIB_PATH -I$INC_PATH

