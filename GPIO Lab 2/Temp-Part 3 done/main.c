#include "FreeRTOS.h"
#include "board_io.h"
#include "common_macros.h"
#include "gpio_lab.h"
#include "periodic_scheduler.h"
#include "semphr.h"
#include "sj2_cli.h"
#include "task.h"
#include <stdbool.h>
#include <stdio.h>

#include <lpc40xx.h>

// 'static' to make these functions 'private' to this file
static void create_blinky_tasks(void);
static void create_uart_task(void);
static void blink_task(void *params);
static void uart_task(void *params);
static void part_0_basic_blink(void *pvParameters);
static void choose_pins_as_io_pins(void);
static void led_task(void *task_parameter);

static SemaphoreHandle_t switch_press_indication;

typedef struct {
  uint8_t port;
  uint8_t pin;
} port_pin_s;

static void choose_pins_as_io_pins(void) {
  uint32_t *iocon_p1_26 = (uint32_t *)0x4002C0E8;
  *iocon_p1_26 &= ~(0b111 << 0);

  uint32_t *iocon_p0_0 = (uint32_t *)0x4002C000;
  *iocon_p0_0 &= ~(0b111 << 0);

  uint32_t *iocon_p0_1 = (uint32_t *)0x4002C004;
  *iocon_p0_1 &= ~(0b111 << 0);

  uint32_t *iocon_p1_19 = (uint32_t *)0x4002C0CC;
  *iocon_p1_19 &= ~(0b111 << 0);
}

void led_task(void *task_parameter) {
  const port_pin_s *led = (port_pin_s *)(task_parameter);

  while (true) {
    // Note: There is no vTaskDelay() here, but we use sleep mechanism while waiting for the binary semaphore (signal)
    if (xSemaphoreTake(switch_press_indication, 1000)) {
      // TODO: Blink the LED
      gpio0__set_high(led->port, led->pin);
      vTaskDelay(1000);

      gpio0__set_low(led->port, led->pin);
      vTaskDelay(1000);
    } else {
      puts("Timeout: No switch press indication for 1000ms");
    }
  }
}

void switch_task(void *task_parameter) {
  port_pin_s *switch1 = (port_pin_s *)task_parameter;
  gpio0__set_as_input(switch1->port, switch1->pin);

  while (true) {
    // TODO: If switch pressed, set the binary semaphore
    while (gpio0__get_level(switch1->port, switch1->pin)) {
      vTaskDelay(50);
      if (!gpio0__get_level(switch1->port, switch1->pin))
        xSemaphoreGive(switch_press_indication);
    }

    // xSemaphoreGive(switch_press_indication);

    // Task should always sleep otherwise they will use 100% CPU
    // This task sleep also helps avoid spurious semaphore give during switch debeounce
    vTaskDelay(100);
  }
}

int main(void) {
  switch_press_indication = xSemaphoreCreateBinary();
  // create_blinky_tasks();
  // create_uart_task();
  choose_pins_as_io_pins();

  static port_pin_s switch1 = {1, 19};
  static port_pin_s led = {1, 26};

  xTaskCreate(switch_task, "sw_task", 4096 / sizeof(void *), &switch1, PRIORITY_LOW, NULL);
  xTaskCreate(led_task, "led_task", 4096 / sizeof(void *), &led, PRIORITY_LOW, NULL);

  // xTaskCreate(part_0_basic_blink, "Blink", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}

static void part_0_basic_blink(void *pvParameters) {

  uint32_t pin26 = (1 << 26);
  uint32_t *port1_pin = (uint32_t *)(0x20098000 + 0x034);

  LPC_GPIO1->DIR |= pin26;

  while (1) {
    LPC_GPIO1->PIN |= pin26;
    vTaskDelay(1000);
    LPC_GPIO1->PIN &= ~pin26;
    vTaskDelay(1000);
    // fprintf(stderr, "%p vs. %p\n", port1_pin, &(LPC_GPIO1->PIN));
    fprintf(stderr, "%p vs. %p vs. %p\n", &(LPC_GPIO1->DIR), &(LPC_GPIO1->PIN), &(LPC_GPIO1->SET));
  }
}

static void create_blinky_tasks(void) {
  /**
   * Use '#if (1)' if you wish to observe how two tasks can blink LEDs
   * Use '#if (0)' if you wish to use the 'periodic_scheduler.h' that will spawn 4 periodic tasks, one for each LED
   */
#if (1)
  // These variables should not go out of scope because the 'blink_task' will reference this memory
  static gpio_s led0, led1;

  led0 = board_io__get_led0();
  led1 = board_io__get_led1();

  xTaskCreate(blink_task, "led0", configMINIMAL_STACK_SIZE, (void *)&led0, PRIORITY_LOW, NULL);
  xTaskCreate(blink_task, "led1", configMINIMAL_STACK_SIZE, (void *)&led1, PRIORITY_LOW, NULL);
#else
  const bool run_1000hz = true;
  const size_t stack_size_bytes = 2048 / sizeof(void *); // RTOS stack size is in terms of 32-bits for ARM M4 32-bit CPU
  periodic_scheduler__initialize(stack_size_bytes, !run_1000hz); // Assuming we do not need the high rate 1000Hz task
  UNUSED(blink_task);
#endif
}

static void create_uart_task(void) {
  // It is advised to either run the uart_task, or the SJ2 command-line (CLI), but not both
  // Change '#if (0)' to '#if (1)' and vice versa to try it out
#if (0)
  // printf() takes more stack space, size this tasks' stack higher
  xTaskCreate(uart_task, "uart", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
#else
  sj2_cli__init();
  UNUSED(uart_task); // uart_task is un-used in if we are doing cli init()
#endif
}

static void blink_task(void *params) {
  const gpio_s led = *((gpio_s *)params); // Parameter was input while calling xTaskCreate()

  // Warning: This task starts with very minimal stack, so do not use printf() API here to avoid stack overflow
  while (true) {
    gpio__toggle(led);
    vTaskDelay(500);
  }
}

// This sends periodic messages over printf() which uses system_calls.c to send them to UART0
static void uart_task(void *params) {
  TickType_t previous_tick = 0;
  TickType_t ticks = 0;

  while (true) {
    // This loop will repeat at precise task delay, even if the logic below takes variable amount of ticks
    vTaskDelayUntil(&previous_tick, 2000);

    /* Calls to fprintf(stderr, ...) uses polled UART driver, so this entire output will be fully
     * sent out before this function returns. See system_calls.c for actual implementation.
     *
     * Use this style print for:
     *  - Interrupts because you cannot use printf() inside an ISR
     *    This is because regular printf() leads down to xQueueSend() that might block
     *    but you cannot block inside an ISR hence the system might crash
     *  - During debugging in case system crashes before all output of printf() is sent
     */
    ticks = xTaskGetTickCount();
    fprintf(stderr, "%u: This is a polled version of printf used for debugging ... finished in", (unsigned)ticks);
    fprintf(stderr, " %lu ticks\n", (xTaskGetTickCount() - ticks));

    /* This deposits data to an outgoing queue and doesn't block the CPU
     * Data will be sent later, but this function would return earlier
     */
    ticks = xTaskGetTickCount();
    printf("This is a more efficient printf ... finished in");
    printf(" %lu ticks\n\n", (xTaskGetTickCount() - ticks));
  }
}
