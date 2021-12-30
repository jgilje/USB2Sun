#include <bsp/board.h>
#include <tusb.h>

#include <pico/stdlib.h>
#include <hardware/uart.h>

static bool serial_data_in_tail = false;
static bool updated = false;
static int32_t delta_x = 0;
static int32_t delta_y = 0;
#define NO_BUTTONS 0x7
static char btns = NO_BUTTONS;
static uint32_t interval = 40;

#define UART_MOUSE_ID uart0
#define UART_MOUSE_TX_PIN 0
void mouse_uart_init() {
  uart_init(UART_MOUSE_ID, 1200);
  gpio_set_function(UART_MOUSE_TX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(UART_MOUSE_ID, false, false);
  uart_set_format(UART_MOUSE_ID, 8, 1, UART_PARITY_NONE);
  gpio_set_outover(UART_MOUSE_TX_PIN, GPIO_OVERRIDE_INVERT);
}

static inline int32_t clamp(int32_t value, int32_t min, int32_t max) {
  if      (value < min) return min;
  else if (value > max) return max;
  return value;
}

static uint32_t push_head_packet() {
  uart_putc_raw(UART_MOUSE_ID, btns | 0x80);
  uart_putc_raw(UART_MOUSE_ID, delta_x);
  uart_putc_raw(UART_MOUSE_ID, delta_y);
  btns = NO_BUTTONS;
  delta_x = 0;
  delta_y = 0;
  serial_data_in_tail = true;
  return 25;
}

static uint32_t push_tail_packet() {
  uart_putc_raw(UART_MOUSE_ID, delta_x);
  uart_putc_raw(UART_MOUSE_ID, delta_y);
  delta_x = 0;
  delta_y = 0;
  serial_data_in_tail = false;
  return 15;
}

void mouse_tx() {
  static uint32_t start_ms = 0;
  if ((board_millis() - start_ms) < interval) {
    return;
  }
  start_ms += interval;

  if (updated) {
    if (serial_data_in_tail) {
      interval = push_tail_packet();
      updated = (btns != NO_BUTTONS);
    } else {
      interval = push_head_packet();
    }
  }
}

void process_mouse_report(hid_mouse_report_t const * report) {
  // for some reason... x is buttons, y is x and wheel is y
  btns = ((report->x & MOUSE_BUTTON_LEFT)   ? 0 : 4)
       | ((report->x & MOUSE_BUTTON_MIDDLE) ? 0 : 2)
       | ((report->x & MOUSE_BUTTON_RIGHT)  ? 0 : 1)
  ;
  delta_x += report->y;
  delta_y += -report->wheel;
  delta_x = clamp(delta_x, -127, 127);
  delta_y = clamp(delta_y, -127, 127);
  updated = true;
}
