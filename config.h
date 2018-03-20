#ifndef __HID_APPLE_CONFIG_H
#define __HID_APPLE_CONFIG_H

#include <linux/input.h>

#include "hid-apple.h"

/*
 * Mode of media keys on Apple keyboards
 * 0 = disabled, media keys are function keys
 * 1 = normally media keys, switchable to function keys by holding fn key
 * 2 = normally function keys, switchable to media keys by holding fn key
 */
static const unsigned int media_key_mode = 1;

/*
 * Mode of numlock on Apple keyboards
 * 0 = disabled, there were no numlock layer
 * 1 = numlock layer enabled only for devices with numlock quirk
 * 2 = numlock layer enabled for all devices
 */
static const unsigned int numlock_mode = 1;

/*
 * Key mappings for normal layer.
 * Fn key could be mapped to another key as well and vice versa.
 *
 * Normal layer will be applied if the keyboard is not in fn layer nor
 * numlock layer.
 *
 * Please consult input.h for the available keys.
 */
static const struct apple_key_translation key_mappings[] = {
//	{ KEY_FN,	KEY_LEFTCTRL },
//	{ KEY_LEFTCTRL,	KEY_FN },
	{ }
};

/*
 * Key mappings for fn layer.
 *
 * This key mappings will have higher precedence than
 * device specific fn mappings.
 */
static const struct apple_key_translation fn_key_mappings[] = {
//	{ KEY_ENTER,	KEY_ENTER },
	{ }
};

/*
 * Key mappings for numlock layer.
 *
 * It is possible to make use of the numlock layer as an addition
 * programmable layer. You are adviced to map numlock, ctrl, alt to
 * accessible keys if you do so.
 */
static const struct apple_key_translation numlock_key_mappings[] = {
	{ KEY_J,	KEY_KP1 },
	{ KEY_K,	KEY_KP2 },
	{ KEY_L,	KEY_KP3 },
	{ KEY_U,	KEY_KP4 },
	{ KEY_I,	KEY_KP5 },
	{ KEY_O,	KEY_KP6 },
	{ KEY_7,	KEY_KP7 },
	{ KEY_8,	KEY_KP8 },
	{ KEY_9,	KEY_KP9 },
	{ KEY_M,	KEY_KP0 },
	{ KEY_DOT,	KEY_KPDOT },
	{ KEY_SLASH,	KEY_KPPLUS },
	{ KEY_SEMICOLON, KEY_KPMINUS },
	{ KEY_P,	KEY_KPASTERISK },
	{ KEY_MINUS,	KEY_KPEQUAL },
	{ KEY_0,	KEY_KPSLASH },
	{ KEY_F6,	KEY_NUMLOCK },
	{ KEY_KPENTER,	KEY_KPENTER },
	{ KEY_BACKSPACE, KEY_BACKSPACE },
	{ }
};

/*
 * Keys mappings for fn layer of general Apple keyboard.
 *
 * Keys mapped by fn_key_mappings will have a higher precedence than
 * device specific fn mappings.
 */
static const struct apple_key_translation apple_fn_keys[] = {
	{ KEY_BACKSPACE, KEY_DELETE },
	{ KEY_ENTER,	KEY_INSERT },
	{ KEY_F1,	KEY_BRIGHTNESSDOWN, APPLE_FLAG_FKEY },
	{ KEY_F2,	KEY_BRIGHTNESSUP,   APPLE_FLAG_FKEY },
	{ KEY_F3,	KEY_SCALE,          APPLE_FLAG_FKEY },
	{ KEY_F4,	KEY_DASHBOARD,      APPLE_FLAG_FKEY },
	{ KEY_F5,	KEY_KBDILLUMDOWN,   APPLE_FLAG_FKEY },
	{ KEY_F6,	KEY_KBDILLUMUP,     APPLE_FLAG_FKEY },
	{ KEY_F7,	KEY_PREVIOUSSONG,   APPLE_FLAG_FKEY },
	{ KEY_F8,	KEY_PLAYPAUSE,      APPLE_FLAG_FKEY },
	{ KEY_F9,	KEY_NEXTSONG,       APPLE_FLAG_FKEY },
	{ KEY_F10,	KEY_MUTE,           APPLE_FLAG_FKEY },
	{ KEY_F11,	KEY_VOLUMEDOWN,     APPLE_FLAG_FKEY },
	{ KEY_F12,	KEY_VOLUMEUP,       APPLE_FLAG_FKEY },
	{ KEY_UP,	KEY_PAGEUP },
	{ KEY_DOWN,	KEY_PAGEDOWN },
	{ KEY_LEFT,	KEY_HOME },
	{ KEY_RIGHT,	KEY_END },
	{ }
};

/*
 * Keys mappings for fn layer of devices with ID between
 * 0x023f and 0x0244 inclusively.
 *
 * Keys mapped by fn_key_mappings will have a higher precedence than
 * device specific fn mappings.
 */
static const struct apple_key_translation macbookair_fn_keys[] = {
	{ KEY_BACKSPACE, KEY_DELETE },
	{ KEY_ENTER,	KEY_INSERT },
	{ KEY_F1,	KEY_BRIGHTNESSDOWN, APPLE_FLAG_FKEY },
	{ KEY_F2,	KEY_BRIGHTNESSUP,   APPLE_FLAG_FKEY },
	{ KEY_F3,	KEY_SCALE,          APPLE_FLAG_FKEY },
	{ KEY_F4,	KEY_DASHBOARD,      APPLE_FLAG_FKEY },
	{ KEY_F6,	KEY_PREVIOUSSONG,   APPLE_FLAG_FKEY },
	{ KEY_F7,	KEY_PLAYPAUSE,      APPLE_FLAG_FKEY },
	{ KEY_F8,	KEY_NEXTSONG,       APPLE_FLAG_FKEY },
	{ KEY_F9,	KEY_MUTE,           APPLE_FLAG_FKEY },
	{ KEY_F10,	KEY_VOLUMEDOWN,     APPLE_FLAG_FKEY },
	{ KEY_F11,	KEY_VOLUMEUP,       APPLE_FLAG_FKEY },
	{ KEY_F12,	KEY_EJECTCD,        APPLE_FLAG_FKEY },
	{ KEY_UP,	KEY_PAGEUP },
	{ KEY_DOWN,	KEY_PAGEDOWN },
	{ KEY_LEFT,	KEY_HOME },
	{ KEY_RIGHT,	KEY_END },
	{ }
};

/*
 * Keys mappings for fn layer of devices not applicable to
 * macbookair_fn_keys while having an ID before 0x21d inclusively or
 * after 0x300 exclusively.
 *
 * Keys mapped by fn_key_mappings will have a higher precendence than
 * device specific fn mappings.
 */
static const struct apple_key_translation powerbook_fn_keys[] = {
	{ KEY_BACKSPACE, KEY_DELETE },
	{ KEY_F1,	KEY_BRIGHTNESSDOWN,     APPLE_FLAG_FKEY },
	{ KEY_F2,	KEY_BRIGHTNESSUP,       APPLE_FLAG_FKEY },
	{ KEY_F3,	KEY_MUTE,               APPLE_FLAG_FKEY },
	{ KEY_F4,	KEY_VOLUMEDOWN,         APPLE_FLAG_FKEY },
	{ KEY_F5,	KEY_VOLUMEUP,           APPLE_FLAG_FKEY },
	{ KEY_F6,	KEY_NUMLOCK,            APPLE_FLAG_FKEY },
	{ KEY_F7,	KEY_SWITCHVIDEOMODE,    APPLE_FLAG_FKEY },
	{ KEY_F8,	KEY_KBDILLUMTOGGLE,     APPLE_FLAG_FKEY },
	{ KEY_F9,	KEY_KBDILLUMDOWN,       APPLE_FLAG_FKEY },
	{ KEY_F10,	KEY_KBDILLUMUP,         APPLE_FLAG_FKEY },
	{ KEY_UP,	KEY_PAGEUP },
	{ KEY_DOWN,	KEY_PAGEDOWN },
	{ KEY_LEFT,	KEY_HOME },
	{ KEY_RIGHT,	KEY_END },
	{ }
};

#endif
