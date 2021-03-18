#include "ssp2_lab.h"
#include "lpc40xx.h"
//#include <stdint.h>

const uint32_t flash_cs_pin = (1 << 10);

void spi2__init(uint32_t max_clock_mhz) {
  // a) Power on Peripheral
  spi2_init_pins();
  spi2_power_on();
  // b) Setup control registers CR0 and CR1
  spi2_setup_control_registers();
  // c) Setup prescalar register to be <= max_clock_mhz
}

void spi2_wait(void) {
  const uint32_t busy_pin = (1 << 4);
  while (LPC_SSP2->SR & busy_pin) {
  }
}

uint8_t spi2__exchange_byte(uint8_t data_out) {

  LPC_SSP2->DR = data_out;
  spi2_wait(); // wait is mandatory because your cpu runs at 96mhz while memory is less than 30.
  // wait ensures that memory can keep up
  // Also ensures bytes fully exchanged before the shifted registers.
  return LPC_SSP2->DR;
}

void spi2_power_on(void) {
  const uint32_t PCSSP2_pin = (1 << 20);
  LPC_SC->PCONP |= PCSSP2_pin;
}

void spi2_setup_control_registers(void) {

  uint32_t cr0_8bit_transfer_pins = 0b0111;
  uint32_t cr0_frame_format_pins = (0b00 << 4);
  uint32_t cr0_scr_division = (0b0100 << 8);
  uint32_t ssp_enable = (0b1 << 1);

  // DSS set to 0b0111 --> 8 bit transfer
  LPC_SSP2->CR0 &= ~(0b1111 << 0);
  LPC_SSP2->CR0 |= cr0_8bit_transfer_pins;

  // Set FRF to 00
  LPC_SSP2->CR0 &= ~(0b11 << 4);

  // Enable SSP
  LPC_SSP2->CR1 |= ssp_enable;

  // Divides 96Mhz by 4.
  // LPC_SSP2->CPSR = 4; --> ***This is the class standard 96 / 4 ***
  LPC_SSP2->CPSR = 24; //We use clock rate of 96 / 24 because our logic analyzer is garbage and is super slow.

}

void spi2_init_pins(void) {
  // !(FLASH_CS) P1_10
  // MISO2 / FLASH + SD/ P1_4
  // SCK2 / FLASH + SD/ P1_0
  // MOSI / FLASH + SD / P1_1

  const uint32_t ssp2_pin_func = 0b100;

  // Reset func pins to all 0
  LPC_IOCON->P1_0 &= (0b111 << 0);
  LPC_IOCON->P1_1 &= (0b111 << 0);
  LPC_IOCON->P1_4 &= (0b111 << 0);
  LPC_IOCON->P1_10 &= (0b111 << 0);

  // Sets all func pins to ssp
  LPC_IOCON->P1_0 |= ssp2_pin_func;
  LPC_IOCON->P1_1 |= ssp2_pin_func;
  LPC_IOCON->P1_4 |= ssp2_pin_func;

  // Configure Chip Select Pin P1_10
  LPC_GPIO1->DIR |= flash_cs_pin; // Sets GPIO to output
  external_flash_deactivate();
}

void external_flash_activate(void) { LPC_GPIO1->CLR = flash_cs_pin; }

void external_flash_deactivate(void) { LPC_GPIO1->SET = flash_cs_pin; }
