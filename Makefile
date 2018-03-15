LINUX_HEADER_DIR ?= /usr/src/linux-headers-$(shell uname -r)
OBJPATH=./obj

obj-$(CONFIG_HID_APPLE)		+= hid-apple.o

all: getobj hid-apple putobj

hid-apple:
	make -C $(LINUX_HEADER_DIR) M=$(CURDIR) modules

putobj:
	mkdir -p $(OBJPATH)
	mv -t $(OBJPATH) .tmp_versions .*.cmd *.mod.c *.o Module.symvers modules.order

getobj:
	if [[ -d "$(OBJPATH)" ]]; then \
		mv $(OBJPATH)/{.[!.],}* .; \
	fi

cleanall:
	make -C $(LINUX_HEADER_DIR) M=$(CURDIR) clean

install:
	make -C $(LINUX_HEADER_DIR) M=$(CURDIR) modules_install
