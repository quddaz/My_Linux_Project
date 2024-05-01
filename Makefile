CR_C := arm-linux-gnueabih-
obj-m = driver.o
KDIR := /mnt/c/Users/김병웅/my_dir/driver.c
PWD := $(shell pwd)
default:
	make ARCH=arm CROSS_COMPILE=$(CR_C) -C $(KDIR) M=$(PWD) modules
clean:
	make ARCH=arm CROSS_COMPILE=$(CR_C) -C $(KDIR) M=$(PWD) clean
