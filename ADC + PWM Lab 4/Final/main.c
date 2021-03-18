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

// ADC + PWM Lab
#include "adc.h"
#include "pwm1.h"
#include "queue.h"

// ADC + PWM Lab
static void initialize_all_led(void);
static void pwm_task_part_0(void *p);
static void pin_configure_pwm_channel_as_io_pin(void);
static void pin_configure_adc_channel_as_io_pin(void);
static void adc_task(void *p);
static void pwm_task_part_2(void *p);
static QueueHandle_t adc_to_pwm_task_queue;

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

static void pwm_task_part_0(void *p) {
  pwm1__init_single_edge(1000);

  // Locate a GPIO pin that a PWM channel will control
  // NOTE You can use gpio__construct_with_function() API from gpio.h
  // TODO Write this function yourself
  pin_configure_pwm_channel_as_io_pin();

  // We only need to set PWM configuration once, and the HW will drive
  // the GPIO at 1000Hz, and control set its duty cycle to 50%
  pwm1__set_duty_cycle(PWM1__2_0, 50);

  // Continue to vary the duty cycle in the loop
  uint8_t percent = 0;
  while (1) {
    pwm1__set_duty_cycle(PWM1__2_0, percent);

    if (++percent > 100) {
      percent = 0;
    }

    vTaskDelay(100);
  }
}

static void pwm_task_part_2(void *p) {
  // NOTE: Reuse the code from Part 0
  int adc_reading = 0;
  pwm1__init_single_edge(1000);
  pin_configure_pwm_channel_as_io_pin();
  pwm1__set_duty_cycle(PWM1__2_0, 100);

  uint8_t percent = 0;
  float voltage = 0;

  while (1) {
    // Implement code to receive potentiometer value from queue
    if (xQueueReceive(adc_to_pwm_task_queue, &adc_reading, 100)) {
      percent = (adc_reading / 41);
      voltage = (3.3 * percent) / 100;
      fprintf(stderr, "Received: %i.  Percent: %i. Voltage: %.2f\n", adc_reading, percent, voltage);
      pwm1__set_duty_cycle(PWM1__2_0, percent);
    }
    // We do not need task delay because our queue API will put task to sleep when there is no data in the queue
    // vTaskDelay(100);
  }
}

static void adc_task(void *p) {

  adc__initialize();
  adc__enable_burst_mode();
  pin_configure_adc_channel_as_io_pin();
  int adc_reading = 0;

  while (1) {
    const uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_2);
    adc_reading = adc_value;
    xQueueSend(adc_to_pwm_task_queue, &adc_reading, 0);
    // fprintf(stderr, "ADC Reading: %i\n", adc_value);
    vTaskDelay(300);
  }
}

void main(void) {
  initialize_all_led();

  adc_to_pwm_task_queue = xQueueCreate(1, sizeof(int));

  xTaskCreate(adc_task, "ADC", 1024, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(pwm_task_part_2, "pwm", 256, NULL, PRIORITY_LOW, NULL);

  // Part of Part 0
  // xTaskCreate(pwm_task_part_0, "pwm", 2048, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
