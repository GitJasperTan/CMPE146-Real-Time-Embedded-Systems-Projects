#pragma once

#include <stdint.h>
#include <stdio.h>

void spi2__init(uint32_t max_clock_mhz);
// Refer to LPC User manual and setup the register bits correctly
// a) Power on Peripheral
// b) Setup control registers CR0 and CR1
// c) Setup prescalar register to be <= max_clock_mhz

uint8_t spi2__exchange_byte(uint8_t data_out);
// Configure the Data register(DR) to send and receive data by checking the SPI peripheral status register

void spi2_power_on(void);
void spi2_setup_control_registers(void);
void spi2_init_pins(void);
void external_flash_activate(void);
void external_flash_deactivate(void);