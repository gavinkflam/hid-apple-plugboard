hid-apple-plugboard
===================

Customizable key mappings for Apple keyboards.

Functionality
-------------

* Support key mappings in the normal layer.
* `fn` key can be mapped to another key and vice versa.
* Support key mappings in the fn layer.
* Support key mappings in the numlock layer.
* Support various media key mode and numlock mode.
  Please refer to `config.h` for explanations.
* Support device specific fn key mappings as the original module supported.
* Support numlock quirks as the original module supported.
* Support devices as the original module supported.

Configuration
-------------

Configuration was done with `config.h`.

You can find descriptions and examples for each configuration items within
the file.

Installation via DKMS (recommended)
-----------------------------------

1. Install module via DKMS

```
sudo dkms add .
sudo dkms build hid-apple-plugboard/1.0.0-rc.1
sudo dkms install hid-apple-plugboard/1.0.0-rc.1
```

2. Override original `hid-apple` module

You will need to create `/etc/depmod.d/hid-apple-plugboard.conf`
as shown below to override the original module.

```
override hid_apple * extra
```

3. Reboot or reload the kernel module

The new kernel will be effective after reboot.

Moreover, you can reload the module for immediate use.

```
sudo modprobe -r hid-apple; sudo modprobe hid-apple
```

Goal
----

Simple and readable. Easily customizable for power user.

Acknowledgement
---------------

This work was inspired by the
[hid-apple-patched](https://github.com/free5lot/hid-apple-patched/)
project by free5lot and other contributors.

The DKMS configuration file and Makefile was based on that of the
hid-apple-patched project.

The source was based on the `hid-apple` module from
[Linux 4.15-rc9](https://github.com/torvalds/linux/blob/v4.15-rc9/drivers/hid/hid-apple.c).

License
-------

GPLv2
