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

static uint8_t hotkey_combo = KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL;
static uint8_t active_key_list[128];
static uint8_t active_keys = 0;

void process_kbd_report(hid_keyboard_report_t const *report) {
  uint8_t current_key_list[128];
  memset(current_key_list, 0, sizeof(current_key_list));

  if (report->modifier != 0) {
    current_key_list[usb2sun[HID_KEY_SHIFT_LEFT]]    = report->modifier & KEYBOARD_MODIFIER_LEFTSHIFT  ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_CONTROL_LEFT]]  = report->modifier & KEYBOARD_MODIFIER_LEFTCTRL   ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_ALT_LEFT]]      = report->modifier & KEYBOARD_MODIFIER_LEFTALT    ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_GUI_LEFT]]      = report->modifier & KEYBOARD_MODIFIER_LEFTGUI    ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_SHIFT_RIGHT]]   = report->modifier & KEYBOARD_MODIFIER_RIGHTSHIFT ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_CONTROL_RIGHT]] = report->modifier & KEYBOARD_MODIFIER_RIGHTCTRL  ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_ALT_RIGHT]]     = report->modifier & KEYBOARD_MODIFIER_RIGHTALT   ? 1 : 0;
    current_key_list[usb2sun[HID_KEY_GUI_RIGHT]]     = report->modifier & KEYBOARD_MODIFIER_RIGHTGUI   ? 1 : 0;
  }

  uint8_t i = 0;
  if (report->modifier == hotkey_combo) {
    current_key_list[usb2sun[HID_KEY_SHIFT_LEFT]] = 0;
    current_key_list[usb2sun[HID_KEY_CONTROL_LEFT]] = 0;
    switch (report->keycode[i]) {
      case HID_KEY_F1:
        current_key_list[SUN_KEY_STOP] = 1;
        break;
      case HID_KEY_F2:
        current_key_list[SUN_KEY_AGAIN] = 1;
        break;
      case HID_KEY_1:
        current_key_list[SUN_KEY_PROPS] = 1;
        break;
      case HID_KEY_2:
        current_key_list[SUN_KEY_UNDO] = 1;
        break;
      case HID_KEY_Q:
        current_key_list[SUN_KEY_FRONT] = 1;
        break;
      case HID_KEY_W:
        current_key_list[SUN_KEY_COPY] = 1;
        break;
      case HID_KEY_A:
        current_key_list[SUN_KEY_OPEN] = 1;
        break;
      case HID_KEY_S:
        current_key_list[SUN_KEY_PASTE] = 1;
        break;
      case HID_KEY_Z:
        current_key_list[SUN_KEY_FIND] = 1;
        break;
      case HID_KEY_X:
        current_key_list[SUN_KEY_CUT] = 1;
        break;
    }
    i++;
  }
  for (; i < 6; i++) {
    if (report->keycode[i]) {
      current_key_list[usb2sun[report->keycode[i]]] = 1;
    }
  }

  for (uint8_t i = 0; i < 128; i++) {
    if (active_key_list[i] && !current_key_list[i]) {
      uart_putc_raw(UART_KBD_ID, i | 0x80);
      active_key_list[i] = 0;
      active_keys--;
    }
    if (!active_key_list[i] && current_key_list[i]) {
      uart_putc_raw(UART_KBD_ID, i);
      active_key_list[i] = 1;
      active_keys++;
    }
  }

  if (active_keys == 0) {
    uart_putc_raw(UART_KBD_ID, 0x7f);
  }
}
