LINUX_HEADER_DIR ?= /lib/modules/$(shell uname -r)/build
OBJPATH ?= ./obj

obj-$(CONFIG_HID_APPLE) += hid-apple.o

# Proxy tasks for kenrel Makefile

modules:
	make -C $(LINUX_HEADER_DIR) M=$(CURDIR) modules

clean:
	make -C $(LINUX_HEADER_DIR) M=$(CURDIR) clean

install:
	make -C $(LINUX_HEADER_DIR) M=$(CURDIR) modules_install

# Development REPL tasks

devbuild: getobj modules putobj

devrepl: devbuild unload load

devclean:
	rm -rf $(OBJPATH) *.ko

load:
	insmod hid-apple.ko

unload:
	-rmmod hid-apple 2>/dev/null

reload: unload load

putobj:
	mkdir -p $(OBJPATH)
	mv -t $(OBJPATH) .tmp_versions .*.cmd *.mod.c *.o Module.symvers modules.order

getobj:
	if [[ -d "$(OBJPATH)" ]]; then \
		mv $(OBJPATH)/{.[!.],}* .; \
	fi
