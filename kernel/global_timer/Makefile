#obj-y += gtimer.o

KDIR := "/home/cylee/alps/final/kernel/linux-3.0.35-imx6-native"
PWD := $(shell pwd)

obj-m := gtimer.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
