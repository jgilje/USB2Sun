/*
 * Sources:
 *
 * https://github.com/xelalexv/suniversal/blob/master/suniversal/sun_to_usb.h
 * Part no. 800-6802-12 - Sun Types Keyboard and Mouse Product Notes
 *  http://vtda.org/docs/computing/Sun/hardware/800-6802-12_Type5KeyboardandMouseProductNotes_RevA_Oct93.pdf
 * http://shikasan.net/sunkey/sunkey_e.html
 */

#ifndef _KEYCODES_H_
#define _KEYCODES_H_

#include <stdint.h>
#include "tusb.h"

static const uint8_t usb2sun[256] = {
  [HID_KEY_HELP] = 0x76,
  [HID_KEY_ESCAPE] = 0x0f,
  [HID_KEY_F1] = 0x05,
  [HID_KEY_F2] = 0x06,
  [HID_KEY_F3] = 0x08,
  [HID_KEY_F4] = 0x0a,
  [HID_KEY_F5] = 0x0c,
  [HID_KEY_F6] = 0x0e,
  [HID_KEY_F7] = 0x10,
  [HID_KEY_F8] = 0x11,
  [HID_KEY_F9] = 0x12,
  [HID_KEY_F10] = 0x07,
  [HID_KEY_F11] = 0x09,
  [HID_KEY_F12] = 0x0b,
  [HID_KEY_PRINT_SCREEN] = 0x16,
  [HID_KEY_SCROLL_LOCK] = 0x17,
  [HID_KEY_PAUSE] = 0x15,
  [HID_KEY_MUTE] = 0x2d,
  [HID_KEY_VOLUME_DOWN] = 0x2,
  [HID_KEY_VOLUME_UP] = 0x4,
  [HID_KEY_POWER] = 0x30,

  [HID_KEY_GRAVE] = 0x2a,
  [HID_KEY_1] = 0x1e,
  [HID_KEY_2] = 0x1f,
  [HID_KEY_3] = 0x20,
  [HID_KEY_4] = 0x21,
  [HID_KEY_5] = 0x22,
  [HID_KEY_6] = 0x23,
  [HID_KEY_7] = 0x24,
  [HID_KEY_8] = 0x25,
  [HID_KEY_9] = 0x26,
  [HID_KEY_0] = 0x27,
  [HID_KEY_MINUS] = 0x28,
  [HID_KEY_EQUAL] = 0x29,
  [HID_KEY_BACKSPACE] = 0x2b,

  [HID_KEY_TAB] = 0x35,
  [HID_KEY_Q] = 0x36,
  [HID_KEY_W] = 0x37,
  [HID_KEY_E] = 0x38,
  [HID_KEY_R] = 0x39,
  [HID_KEY_T] = 0x3a,
  [HID_KEY_Y] = 0x3b,
  [HID_KEY_U] = 0x3c,
  [HID_KEY_I] = 0x3d,
  [HID_KEY_O] = 0x3e,
  [HID_KEY_P] = 0x3f,
  [HID_KEY_BRACKET_LEFT] = 0x40,
  [HID_KEY_BRACKET_RIGHT] = 0x41,
  [HID_KEY_BACKSLASH] = 0x58,

  [HID_KEY_CAPS_LOCK] = 0x77,
  [HID_KEY_A] = 0x4d,
  [HID_KEY_S] = 0x4e,
  [HID_KEY_D] = 0x4f,
  [HID_KEY_F] = 0x50,
  [HID_KEY_G] = 0x51,
  [HID_KEY_H] = 0x52,
  [HID_KEY_J] = 0x53,
  [HID_KEY_K] = 0x54,
  [HID_KEY_L] = 0x55,
  [HID_KEY_SEMICOLON] = 0x56,
  [HID_KEY_APOSTROPHE] = 0x57,
  [HID_KEY_ENTER] = 0x59,

  [HID_KEY_SHIFT_LEFT] = 0x63,
  [HID_KEY_Z] = 0x64,
  [HID_KEY_X] = 0x65,
  [HID_KEY_C] = 0x66,
  [HID_KEY_V] = 0x67,
  [HID_KEY_B] = 0x68,
  [HID_KEY_N] = 0x69,
  [HID_KEY_M] = 0x6a,
  [HID_KEY_COMMA] = 0x6b,
  [HID_KEY_PERIOD] = 0x6c,
  [HID_KEY_SLASH] = 0x6d,
  [HID_KEY_SHIFT_RIGHT] = 0x6e,

  [HID_KEY_CONTROL_LEFT] = 0x4c,
  [HID_KEY_GUI_LEFT] = 0x78,      // left triangle
  [HID_KEY_ALT_LEFT] = 0x13,
  [HID_KEY_SPACE] = 0x79,
  [HID_KEY_ALT_RIGHT] = 0x0d,     // alt graph
  [HID_KEY_GUI_RIGHT] = 0x7a,     // right triangle
  [HID_KEY_CONTROL_RIGHT] = 0x43, // compose

  [HID_KEY_INSERT] = 0x2c,
  [HID_KEY_HOME] = 0x34,
  [HID_KEY_PAGE_UP] = 0x60,
  [HID_KEY_DELETE] = 0x42,
  [HID_KEY_END] = 0x4a,
  [HID_KEY_PAGE_DOWN] = 0x7b,

  [HID_KEY_ARROW_UP] = 0x14,
  [HID_KEY_ARROW_LEFT] = 0x18,
  [HID_KEY_ARROW_DOWN] = 0x1b,
  [HID_KEY_ARROW_RIGHT] = 0x1c,

  [HID_KEY_NUM_LOCK] = 0x62,
  [HID_KEY_KEYPAD_DIVIDE] = 0x2e,
  [HID_KEY_KEYPAD_MULTIPLY] = 0x2f,
  [HID_KEY_KEYPAD_SUBTRACT] = 0x47,
  [HID_KEY_KEYPAD_ADD] = 0x47,
  [HID_KEY_KEYPAD_ENTER] = 0x5a,
  [HID_KEY_KEYPAD_COMMA] = 0x32,
  [HID_KEY_KEYPAD_0] = 0x5e,
  [HID_KEY_KEYPAD_1] = 0x70,
  [HID_KEY_KEYPAD_2] = 0x71,
  [HID_KEY_KEYPAD_3] = 0x72,
  [HID_KEY_KEYPAD_4] = 0x5b,
  [HID_KEY_KEYPAD_5] = 0x5c,
  [HID_KEY_KEYPAD_6] = 0x5d,
  [HID_KEY_KEYPAD_7] = 0x44,
  [HID_KEY_KEYPAD_8] = 0x45,
  [HID_KEY_KEYPAD_9] = 0x46,
};

#endif
