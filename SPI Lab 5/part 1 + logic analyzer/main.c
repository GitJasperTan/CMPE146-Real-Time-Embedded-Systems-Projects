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
void adesto_cs(void);
void adesto_ds(void);

typedef struct {
  uint8_t port;
  uint8_t pin;
} port_pin_s;

static port_pin_s led[4] = {{2, 3}, {1, 26}, {1, 24}, {1, 18}};

// TODO: Study the Adesto flash 'Manufacturer and Device ID' section
typedef struct {
  uint8_t manufacturer_id;
  uint8_t device_id_1;
  uint8_t device_id_2;
  uint8_t extended_device_id;
} adesto_flash_id_s;

static void initialize_all_led(void) {
  // LED's are active low.  Therefore setting these pins to high turns them all off.
  gpio0__set_high(led[0].port, led[0].pin);
  gpio0__set_high(led[1].port, led[1].pin);
  gpio0__set_high(led[2].port, led[2].pin);
  gpio0__set_high(led[3].port, led[3].pin);
}

static port_pin_s temp_pin = {0, 6};

void adesto_cs(void) { external_flash_activate(); }

void adesto_ds(void) { external_flash_deactivate(); }

// TODO: Implement the code to read Adesto flash memory signature
// TODO: Create struct of type 'adesto_flash_id_s' and return it
adesto_flash_id_s adesto_read_signature(void) {
  adesto_flash_id_s data = {0};
  const uint8_t filler_byte_data = 0xFF;

  adesto_cs();
  LPC_GPIO0->SET = (1 << temp_pin.pin);
  // Send opcode and read bytes
  // TODO: Populate members of the 'adesto_flash_id_s' struct

  (void)spi2__exchange_byte(
      0x9F); // Sends opcode first.  //(void) serves to indicate that this function call returns nothing of importance.
  data.manufacturer_id = spi2__exchange_byte(filler_byte_data);
  data.device_id_1 = spi2__exchange_byte(filler_byte_data);
  data.device_id_2 = spi2__exchange_byte(filler_byte_data);
  data.extended_device_id = spi2__exchange_byte(filler_byte_data);
  adesto_ds();
  LPC_GPIO0->CLR = (1 << temp_pin.pin);

  return data;
}

void spi_task(void *p) {
  const uint32_t spi_clock_mhz = 24;
  // ssp2__init(spi_clock_mhz);

  // From the LPC schematics pdf, find the pin numbers connected to flash memory
  // Read table 84 from LPC User Manual and configure PIN functions for SPI2 pins
  // You can use gpio__construct_with_function() API from gpio.h
  //
  // Note: Configure only SCK2, MOSI2, MISO2.
  // CS will be a GPIO output pin(configure and setup direction)
  // todo_configure_your_ssp2_pin_functions();
  spi2__init(spi_clock_mhz);

  while (1) {
    adesto_flash_id_s id = adesto_read_signature();
    // TODO: printf the members of the 'adesto_flash_id_s' struct
    fprintf(
        stderr,
        "Manufacture ID: 0x%02x\nDevice id 1 : 0x %02x\n Device id 2 : 0x % 02x\n Extended device id : 0x % 02x\n\n ",
        id.manufacturer_id, id.device_id_1, id.device_id_2, id.extended_device_id);

    vTaskDelay(500);
  }
}

void main(void) {
  initialize_all_led();

  LPC_GPIO0->DIR |= (1 << temp_pin.pin);
  LPC_GPIO0->CLR = (1 << temp_pin.pin);

  // Part of Part 0
  // xTaskCreate(pwm_task_part_0, "pwm", 2048, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(spi_task, "spi_task", 2048, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
