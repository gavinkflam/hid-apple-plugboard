#ifndef __HID_APPLE_H
#define __HID_APPLE_H

#include <linux/device.h>

#define APPLE_FLAG_FKEY		0x01

struct apple_sc {
	unsigned long quirks;
	unsigned int fn_on;
	DECLARE_BITMAP(pressed_fn, KEY_CNT);
	DECLARE_BITMAP(pressed_numlock, KEY_CNT);
};

struct apple_key_translation {
	u16 from;
	u16 to;
	u8 flags;
};

#endif
