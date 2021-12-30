#include <tusb.h>

#include <pico/stdlib.h>
#include <hardware/uart.h>
#include <hardware/irq.h>

#include "keycodes.h"

#define UART_KBD_ID uart1
#define UART_KBD_IRQ UART1_IRQ
#define UART_KBD_TX_PIN 4
#define UART_KBD_RX_PIN 5

void on_keyboard_rx();

void keyboard_uart_init() {
  uart_init(UART_KBD_ID, 1200);
  gpio_set_function(UART_KBD_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_KBD_RX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(UART_KBD_ID, false, false);
  uart_set_format(UART_KBD_ID, 8, 1, UART_PARITY_NONE);
  irq_set_exclusive_handler(UART_KBD_IRQ, on_keyboard_rx);
  irq_set_enabled(UART_KBD_IRQ, true);
  uart_set_irq_enables(UART_KBD_ID, true, false);
  gpio_set_inover(UART_KBD_RX_PIN, GPIO_OVERRIDE_INVERT);
  gpio_set_outover(UART_KBD_TX_PIN, GPIO_OVERRIDE_INVERT);
}

// RX interrupt handler
void on_keyboard_rx() {
    while (uart_is_readable(UART_KBD_ID)) {
        // printf("System command: ");
        uint8_t ch = uart_getc(UART_KBD_ID);
        // Can we send it back?

        switch (ch) {
          case 0x01: // reset
            // printf("Reset\n");
            uart_putc_raw(UART_KBD_ID, 0xff);
            uart_putc_raw(UART_KBD_ID, 0x04);
            uart_putc_raw(UART_KBD_ID, 0x7f);
            break;
          case 0x02: // bell on
            // printf("Bell on\n");
            break;
          case 0x03: // bell off
            // printf("Bell off\n");
            break;
          case 0x0a: // click on
            // printf("Click on\n");
            break;
          case 0x0b: // click off
            // printf("Click off\n");
            break;
          case 0x0e: // led command
            // printf("Led\n");
            {
              uint8_t led = uart_getc(UART_KBD_ID);
            }
            break;
          case 0x0f: // layout command
            // printf("Layout\n");
            uart_putc_raw(UART_KBD_ID, 0xfe);
            uart_putc_raw(UART_KBD_ID, 0x00);
            break;
          default:
            // printf("Unknown system command: 0x%02x\n", ch);
            break;
        };
    }
}

static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode) {
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keycode[i] == keycode) {
      return true;
    }
  }

  return false;
}

static uint8_t handle_modifier_press(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev, uint8_t modifier, uint8_t key) {
  if ((report->modifier & modifier) && ((prev->modifier & modifier) == 0)) {
    uint8_t ch = usb2sun[key];
    // printf("make (mod) 0x%x => 0x%x\n", key, ch);
    uart_putc_raw(UART_KBD_ID, ch);
    return 1;
  }
  return 0;
}

static uint8_t handle_modifier_release(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev, uint8_t modifier, uint8_t key) {
  if (((report->modifier & modifier) == 0) && (prev->modifier & modifier)) {
    uint8_t ch = usb2sun[key] | 0x80;
    // printf("break (mod) 0x%x => 0x%x\n", key, ch);
    uart_putc_raw(UART_KBD_ID, ch);
    return 1;
  }
  return 0;
}

void process_kbd_report(hid_keyboard_report_t const *report) {
  static hid_keyboard_report_t prev_report = { 0, 0, {0} };
  int active_keys = __builtin_popcount(report->modifier);// + __builtin_popcount(prev_report.modifier);

  // make codes
  if (report->modifier != 0) {
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_SHIFT_LEFT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_LEFTCTRL, HID_KEY_CONTROL_LEFT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_LEFTALT, HID_KEY_ALT_LEFT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_LEFTGUI, HID_KEY_GUI_LEFT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_RIGHTSHIFT, HID_KEY_SHIFT_RIGHT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_RIGHTCTRL, HID_KEY_CONTROL_RIGHT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_RIGHTALT, HID_KEY_ALT_RIGHT);
    handle_modifier_press(report, &prev_report, KEYBOARD_MODIFIER_RIGHTGUI, HID_KEY_GUI_RIGHT);
  }
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keycode[i]) {
      if (! find_key_in_report(&prev_report, report->keycode[i])) {
        uint8_t ch = usb2sun[report->keycode[i]];
        // printf("make 0x%x => 0x%x\n", report->keycode[i], ch);
        uart_putc_raw(UART_KBD_ID, ch);
        active_keys++;
      }
    }
  }

  // break codes
  if (report->modifier != prev_report.modifier) {
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_SHIFT_LEFT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_LEFTCTRL, HID_KEY_CONTROL_LEFT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_LEFTALT, HID_KEY_ALT_LEFT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_LEFTGUI, HID_KEY_GUI_LEFT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_RIGHTSHIFT, HID_KEY_SHIFT_RIGHT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_RIGHTCTRL, HID_KEY_CONTROL_RIGHT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_RIGHTALT, HID_KEY_ALT_RIGHT);
    handle_modifier_release(report, &prev_report, KEYBOARD_MODIFIER_RIGHTGUI, HID_KEY_GUI_RIGHT);
  }
  for (uint8_t i = 0; i < 6; i++) {
    if (prev_report.keycode[i]) {
      active_keys++;
      if (! find_key_in_report(report, prev_report.keycode[i])) {
        uint8_t ch = usb2sun[prev_report.keycode[i]] | 0x80;
        // printf("break 0x%x => 0x%x\n", prev_report.keycode[i], ch);
        uart_putc_raw(UART_KBD_ID, ch);
        active_keys--;
      }
    }
  }

  if (active_keys == 0) {
    // printf("idle\n");
    uart_putc_raw(UART_KBD_ID, 0x7f);
  }

  prev_report = *report;
}
