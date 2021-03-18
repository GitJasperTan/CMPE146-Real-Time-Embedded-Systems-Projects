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

// UART Lab
#include "uart_lab.h"
void uart_read_task_interrupt(void *p);
void uart_write_task_interrupt(void *p);
void uart_read_task_polled(void *p);
void uart_write_task_polled(void *p);

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

static void initialize_uart_pins(void) {

  // U2_TXD = P2_8
  // U2_RXD = P2_9

  // Initialize IOCON registers
  LPC_IOCON->P2_8 &= ~(0b111 << 0);
  LPC_IOCON->P2_8 |= (1 << 1);

  LPC_IOCON->P2_9 &= ~(0b111 << 0);
  LPC_IOCON->P2_9 |= (1 << 1);
}

void main(void) {
  initialize_all_led();

  uart_lab__init(UART_2, 96U * 1000 * 1000, 38400);
  initialize_uart_pins();
  uart__enable_receive_interrupt(UART_2);

  // Task 1
  // xTaskCreate(uart_read_task_polled, "uart_read_task", 2048, NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(uart_write_task_polled, "uart_write_task", 2048, NULL, PRIORITY_LOW, NULL);

  // Task 2
  xTaskCreate(uart_read_task_interrupt, "uart_read_task", 2048, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(uart_write_task_interrupt, "uart_write_task", 2048, NULL, PRIORITY_LOW, NULL);

  // xTaskCreate(spi_id_verification_task, "spi_task", 2048, &task2, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}

void uart_read_task_interrupt(void *p) {

  char *temp;
  while (1) {
    if (uart_lab__get_char_from_queue(&temp, 1000)) {
      fprintf(stderr, "Interrupt Received: %c\n", temp);
      vTaskDelay(500);
    }
  }
}

void uart_write_task_interrupt(void *p) {

  char temp = 'A';
  while (1) {

    for (char temp = 'A'; temp < 'F'; temp++) {

      uart_lab__polled_put(UART_2, temp);
      vTaskDelay(500);
    }
  }
}

void uart_read_task_polled(void *p) {
  char *temp;
  while (1) {
    uart_lab__polled_get(UART_2, &temp);
    fprintf(stderr, "Read byte: %c\n", temp);
    vTaskDelay(500);
  }
}

void uart_write_task_polled(void *p) {

  char temp = 'B';
  while (1) {
    // fprintf(stderr, "Written byte: %c\n", temp);
    uart_lab__polled_put(UART_2, temp);
    vTaskDelay(500);
  }
}
