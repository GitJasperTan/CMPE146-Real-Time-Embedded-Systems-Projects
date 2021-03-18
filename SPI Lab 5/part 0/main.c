#include "FreeRTOS.h"
#include "board_io.h"
#include "common_macros.h"
#include "gpio_isr.h"
#include "gpio_lab.h"
#include "lpc_peripherals.h"
#include "periodic_scheduler.h"
#include "semphr.h"
#include "sj2_cli.h"
#include "task.h"
#include <lpc40xx.h>
#include <stdbool.h>
#include <stdio.h>
static void initialize_all_led(void);

// SPI Lab
#include "ssp2_lab.h"

typedef struct {
  uint8_t port;
  uint8_t pin;
} port_pin_s;

static port_pin_s led[4] = {{2, 3}, {1, 26}, {1, 24}, {1, 18}};

static void initialize_all_led(void) {
  // LED's are active low.  Therefore setting these pins to high turns them all off.
  gpio0__set_high(led[0].port, led[0].pin);
  gpio0__set_high(led[1].port, led[1].pin);
  gpio0__set_high(led[2].port, led[2].pin);
  gpio0__set_high(led[3].port, led[3].pin);
}

static void pin_configure_pwm_channel_as_io_pin(void) {
  uint32_t *iocon_p2_0 = (uint32_t *)0x4002C100;

  *iocon_p2_0 &= ~(0b111 << 0);
  *iocon_p2_0 |= (1 << 0);
}

static void pin_configure_adc_channel_as_io_pin(void) {
  // Configure Port0 Pin 22 as ADC IO pin;

  // Sets function to ADC
  LPC_IOCON->P0_25 &= ~(0b111 << 0);

  // Sets ADMODE to Analog.  Refer to Table 91 in the manual
  LPC_IOCON->P0_25 &= ~(1 << 7);

  LPC_IOCON->P0_25 |= (1 << 0);
}

void main(void) {
  initialize_all_led();

  // Part of Part 0
  // xTaskCreate(pwm_task_part_0, "pwm", 2048, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
