# hid-apple-plugboard

Customizable key mappings for Apple and Macbook keyboards.

![enigma-plugboard][enigma-plugboard]

## Functionality

* Support key mappings in normal layer.
* `fn` key can be mapped to another key and vice versa.
* Support key mappings in fn layer.
* Support key mappings in numlock layer.
* Support various media key mode and numlock mode.
* Support device specific fn key mappings as the original module supported.
* Support numlock quirks as the original module supported.
* Support devices as the original module supported.

## Configuration

Configuration was done with `config.h`.

You can read the file to find descriptions and examples.

## Installation via package manager (recommended)

You are recommended to create a package with your own
configurations/modifications then install with the package manager.

You can find the examples provided as follows.

Arch Linux:

- [hid-apple-plugboard-git-dkms][hid-apple-plugboard-git-dkms]
- [hid-apple-kestrel-git-dkms][hid-apple-kestrel-git-dkms]

## Installation via DKMS

1. Install module via DKMS

  ```
  sudo dkms add .
  sudo dkms build hid-apple-plugboard/4.18.0
  sudo dkms install hid-apple-plugboard/4.18.0
  ```

2. Override original `hid-apple` module

  You will need to create `/etc/depmod.d/hid-apple-plugboard.conf`
  to override the original module.

  ```
  override hid_apple * extra
  ```

3. Reboot or reload the kernel module

  The new kernel will be effective after reboot.

  Moreover, you can reload the module for immediate use.

  ```
  sudo modprobe -r hid-apple; sudo modprobe hid-apple
  ```

## Goal

Simple, minimal and readable. Easily customizable for power user.

## Acknowledgement

The kernel module was based on Linux 4.18 hid-apple module licensed under GPLv2.

* [hid-apple.c][hid-apple.c]
* [hid-ids.h][hid-ids.h]

The DKMS module and makefile was based on [hid-apple-patched][hid-apple-patched]
by free5lot and other contributors licensed under GPLv2.

Enigma plugboard photo by Bob Lord licensed under [GFDL][gfdl] or
[CC-BY-SA-3.0][cc-by-sa-3-0] via Wikimedia Commons.

## License

GPLv2

[enigma-plugboard]: https://upload.wikimedia.org/wikipedia/commons/thumb/2/27/Enigma-plugboard.jpg/640px-Enigma-plugboard.jpg
[hid-apple-plugboard-git-dkms]: https://gitlab.com/gavinkflam/hid-apple-plugboard-git-dkms
[hid-apple-kestrel-git-dkms]: https://gitlab.com/gavinkflam/hid-apple-kestrel-git-dkms
[hid-apple-patched]: https://github.com/free5lot/hid-apple-patched/
[hid-apple.c]: https://github.com/torvalds/linux/blob/v4.18/drivers/hid/hid-apple.c
[hid-ids.h]: https://github.com/torvalds/linux/blob/v4.18/drivers/hid/hid-ids.h
[gfdl]: http://www.gnu.org/copyleft/fdl.html
[cc-by-sa-3-0]: http://creativecommons.org/licenses/by-sa/3.0/
