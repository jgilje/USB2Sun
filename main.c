/*
 * USB2Sun
 *  Connect a USB keyboard and mouse to your Sun workstation!
 *
 * Joakim L. Gilje
 * Adapted from the TinyUSB Host examples
 */

#include <bsp/board.h>
#include <tusb.h>

extern void keyboard_uart_init();
extern void mouse_uart_init();
extern void mouse_tx();
void led_blinking_task(void);

int main(void) {
  board_init();
  tusb_init();

  keyboard_uart_init();
  mouse_uart_init();

  while (true) {
    tuh_task();
    led_blinking_task();

    mouse_tx();
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

  // Blink every interval ms
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
