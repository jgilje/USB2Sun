/*
 * USB2Sun
 *  Connect a USB keyboard and mouse to your Sun workstation!
 *
 * Joakim L. Gilje
 * Adapted from the TinyUSB Host examples
 */


#include <stdio.h>

#include <bsp/board.h>
#include <tusb.h>

#include "hardware/watchdog.h"
#include "pico/stdlib.h"

extern void keyboard_uart_init();
extern void mouse_uart_init();
extern void mouse_tx();
void led_blinking_task(void);

int last_keyboard_connected = 0;
int keyboard_connected = 0;
static int watchdog_enabled = 0;

int main(void) {
  board_init();

  tuh_init(BOARD_TUH_RHPORT);

  keyboard_uart_init();
  mouse_uart_init();
  printf("USB2Sun Init\n");

  while (true) {
    tuh_task();
    led_blinking_task();

    mouse_tx();
    if (board_millis() > 15000 && keyboard_connected == 0 && watchdog_enabled == 0) {
        printf("No USB devices, rebooting via watchdog\n");
        watchdog_enable(100, 1);
        watchdog_enabled = 1;
    }
  }

  return 0;
}

//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  if (last_keyboard_connected != keyboard_connected) {
    for (int i = 0; i < 3; ++i) {
      board_led_write(1);
      sleep_ms(50);
      board_led_write(0);
      sleep_ms(50);
    }
    last_keyboard_connected = keyboard_connected;
    return;
  }

  if (keyboard_connected) {
    if (board_millis() - start_ms < 2000) {
      board_led_write(0);
    } else if (board_millis() - start_ms < 2100) {
      board_led_write(1);
    } else {
      start_ms = board_millis();
    }
    return;
  }

  // Blink every interval ms
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

void tuh_mount_cb(uint8_t dev_addr)
{
  // application set-up
  printf("\n\nA device with address %d is mounted\n\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr)
{
  // application tear-down
  printf("\n\nA device with address %d is unmounted \r\n", dev_addr);
}
