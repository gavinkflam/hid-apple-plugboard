#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "hid-apple.h"
#include "quirks.h"
#include "config.h"

static const struct apple_key_translation *apple_find_translation(
		const struct apple_key_translation *table, u16 from)
{
	const struct apple_key_translation *trans;

	/* Look for the translation */
	for (trans = table; trans->from; trans++)
		if (trans->from == from)
			return trans;

	return NULL;
}

static int hidinput_apple_event(struct hid_device *hid, struct input_dev *input,
		struct hid_usage *usage, __s32 value)
{
	struct apple_sc *asc = hid_get_drvdata(hid);
	const struct apple_key_translation *trans, *table;
	int do_translate;

	/*
	 * Do key mapping for normal layer.
	 *
	 * User may have mapped fn key to another key / another key to fn key.
	 * Thus the fn key check should be based on the final mapped key.
	 */
	trans = apple_find_translation(key_mappings, usage->code);

	if ((trans ? trans->to : usage->code) == KEY_FN) {
		asc->fn_on = !!value;
		input_event(input, usage->type, KEY_FN, value);
		return 1;
	}

	if (!(test_bit(usage->code, asc->pressed_fn) || asc->fn_on) &&
			trans) {
		input_event(input, usage->type, trans->to,
				value);
		return 1;
	}

	/*
	 * Do key mapping for fn layer.
	 *
	 * The key will be mapped with fn_key_mappings first.
	 * Keys mapped by fn_key_mappings already will not be mapped with device
	 * specific mapping table.
	 */
	trans = apple_find_translation(fn_key_mappings, usage->code);

	if (!trans) {
		if (hid->product >= USB_DEVICE_ID_APPLE_WELLSPRING4_ANSI &&
				hid->product <= USB_DEVICE_ID_APPLE_WELLSPRING4A_JIS)
			table = macbookair_fn_keys;
		else if (hid->product < 0x21d || hid->product >= 0x300)
			table = powerbook_fn_keys;
		else
			table = apple_fn_keys;

		trans = apple_find_translation(table, usage->code);
	}

	/*
	 * Apply fn layer mapping according to fn key state and media key mode.
	 */
	if (trans) {
		if (test_bit(usage->code, asc->pressed_fn))
			do_translate = 1;
		else if (trans->flags & APPLE_FLAG_FKEY)
			do_translate = (media_key_mode == 2 && asc->fn_on) ||
				(media_key_mode == 1 && !asc->fn_on);
		else
			do_translate = asc->fn_on;

		if (do_translate) {
			if (value)
				set_bit(usage->code, asc->pressed_fn);
			else
				clear_bit(usage->code, asc->pressed_fn);

			input_event(input, usage->type, trans->to,
					value);

			return 1;
		}
	}

	/*
	 * Do key mapping for numlock emulation quirk if applicable.
	 */
	if (asc->quirks & APPLE_NUMLOCK_EMULATION &&
			(test_bit(usage->code, asc->pressed_numlock) ||
			test_bit(LED_NUML, input->led))) {
		trans = apple_find_translation(powerbook_numlock_keys,
				usage->code);

		if (trans) {
			if (value)
				set_bit(usage->code,
						asc->pressed_numlock);
			else
				clear_bit(usage->code,
						asc->pressed_numlock);

			input_event(input, usage->type, trans->to,
					value);
		}

		return 1;
	}

	return 0;
}

static int apple_event(struct hid_device *hdev, struct hid_field *field,
		struct hid_usage *usage, __s32 value)
{
	struct apple_sc *asc = hid_get_drvdata(hdev);

	if (!(hdev->claimed & HID_CLAIMED_INPUT) || !field->hidinput ||
			!usage->type)
		return 0;

	if ((asc->quirks & APPLE_INVERT_HWHEEL) &&
			usage->code == REL_HWHEEL) {
		input_event(field->hidinput->input, usage->type, usage->code,
				-value);
		return 1;
	}

	if ((asc->quirks & APPLE_HAS_FN) &&
			hidinput_apple_event(hdev, field->hidinput->input,
				usage, value))
		return 1;

	return 0;
}

/*
 * MacBook JIS keyboard has wrong logical maximum
 */
static __u8 *apple_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
	struct apple_sc *asc = hid_get_drvdata(hdev);

	if ((asc->quirks & APPLE_RDESC_JIS) && *rsize >= 60 &&
			rdesc[53] == 0x65 && rdesc[59] == 0x65) {
		hid_info(hdev,
			 "fixing up MacBook JIS keyboard report descriptor\n");
		rdesc[53] = rdesc[59] = 0xe7;
	}
	return rdesc;
}

static void apple_setup_input(struct input_dev *input)
{
	const struct apple_key_translation *trans;

	set_bit(KEY_NUMLOCK, input->keybit);

	/* Enable all needed keys */
	for (trans = key_mappings; trans->from; trans++) {
		if (trans->to != KEY_FN)
			set_bit(trans->to, input->keybit);
	}

	for (trans = fn_key_mappings; trans->from; trans++)
		set_bit(trans->to, input->keybit);

	for (trans = apple_fn_keys; trans->from; trans++)
		set_bit(trans->to, input->keybit);

	for (trans = powerbook_fn_keys; trans->from; trans++)
		set_bit(trans->to, input->keybit);

	for (trans = powerbook_numlock_keys; trans->from; trans++)
		set_bit(trans->to, input->keybit);
}

static int apple_input_mapping(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	if (usage->hid == (HID_UP_CUSTOM | 0x0003)) {
		/* The fn key on Apple USB keyboards */
		set_bit(EV_REP, hi->input->evbit);
		hid_map_usage_clear(hi, usage, bit, max, EV_KEY, KEY_FN);
		apple_setup_input(hi->input);
		return 1;
	}

	/* we want the hid layer to go through standard path (set and ignore) */
	return 0;
}

static int apple_input_mapped(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	struct apple_sc *asc = hid_get_drvdata(hdev);

	if (asc->quirks & APPLE_MIGHTYMOUSE) {
		if (usage->hid == HID_GD_Z)
			hid_map_usage(hi, usage, bit, max, EV_REL, REL_HWHEEL);
		else if (usage->code == BTN_1)
			hid_map_usage(hi, usage, bit, max, EV_KEY, BTN_2);
		else if (usage->code == BTN_2)
			hid_map_usage(hi, usage, bit, max, EV_KEY, BTN_1);
	}

	return 0;
}

static int apple_probe(struct hid_device *hdev,
		const struct hid_device_id *id)
{
	unsigned long quirks = id->driver_data;
	struct apple_sc *asc;
	unsigned int connect_mask = HID_CONNECT_DEFAULT;
	int ret;

	asc = devm_kzalloc(&hdev->dev, sizeof(*asc), GFP_KERNEL);
	if (asc == NULL) {
		hid_err(hdev, "can't alloc apple descriptor\n");
		return -ENOMEM;
	}

	asc->quirks = quirks;

	hid_set_drvdata(hdev, asc);

	ret = hid_parse(hdev);
	if (ret) {
		hid_err(hdev, "parse failed\n");
		return ret;
	}

	if (quirks & APPLE_HIDDEV)
		connect_mask |= HID_CONNECT_HIDDEV_FORCE;
	if (quirks & APPLE_IGNORE_HIDINPUT)
		connect_mask &= ~HID_CONNECT_HIDINPUT;

	ret = hid_hw_start(hdev, connect_mask);
	if (ret) {
		hid_err(hdev, "hw start failed\n");
		return ret;
	}

	return 0;
}

MODULE_DEVICE_TABLE(hid, apple_devices);

static struct hid_driver apple_driver = {
	.name = "apple",
	.id_table = apple_devices,
	.report_fixup = apple_report_fixup,
	.probe = apple_probe,
	.event = apple_event,
	.input_mapping = apple_input_mapping,
	.input_mapped = apple_input_mapped,
};
module_hid_driver(apple_driver);

MODULE_LICENSE("GPL");
