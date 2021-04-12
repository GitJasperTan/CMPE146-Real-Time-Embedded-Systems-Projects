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

// Producer Consumer Lab
void producer(void *p);
void consumer(void *p);
static QueueHandle_t switch_queue;

typedef enum { switch__off, switch__on } switch_e;

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

static void set_p0_30_as_switch(void) {
  uint32_t p0_30_pin = (1 << 30);
  // Set as GPIO
  LPC_IOCON->P0_30 &= ~(0b111 << 0);

  // Set as input
  LPC_GPIO0->DIR &= ~p0_30_pin;
}

void producer(void *p) {
  uint32_t set_p0_30_pin = (1 << 30);

  while (1) {
    const switch_e switch_value = !(!(LPC_GPIO0->PIN & set_p0_30_pin)); // May need to invert? "!"
    // fprintf(stderr, "Switch value: %i\n", switch_value); //Diagnostic fprintf

    printf("X Before Sending\n");
    xQueueSend(switch_queue, &switch_value, 0);
    printf("X After Sent\n");

    vTaskDelay(1000);
  }
}

void consumer(void *p) {
  switch_e switch_value;

  while (1) {
    printf("Y Before Receiving\n");
    xQueueReceive(switch_queue, &switch_value, 10);

    printf("Y Queue Received\n");

    // printf("%s(), line %d, sending message\n", __FUNCTION__, __LINE__);
  }
}

void main(void) {
  initialize_all_led();
  set_p0_30_as_switch();

  fprintf(stderr, "Consumer Priority LOW\n");
  fprintf(stderr, "Producer Priority LOW\n\n");
  xTaskCreate(producer, "producer", 2048, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(consumer, "producer", 2048, NULL, PRIORITY_LOW, NULL);

  switch_queue = xQueueCreate(1, sizeof(switch_e));

  // xTaskCreate(spi_id_verification_task, "spi_task", 2048, &task2, PRIORITY_LOW, NULL);
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
