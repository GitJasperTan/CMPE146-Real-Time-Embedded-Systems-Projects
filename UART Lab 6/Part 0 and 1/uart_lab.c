#include "uart_lab.h"
#include "lpc40xx.h"
#include "stdio.h"

void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {

  // Baud = 96 / (16 * (DLM * 256 + DLL))
  // Baud = 96 / (16 * ((DLM << 8) + DLL)
  // Baud = 96 / (16 * value_16 bits)

  if (uart == UART_2) {
    // UART2 configurations
    enable_UART2_power();
    set_DLAB_1_UART2();
    set_UART2_8bit();
    disable_fractional_div_UART2();
    set_DLM_DLL_UART2(baud_rate);
    disable_DLAB_1_UART2();
    // enable_FIFO_UART2();

  } else if (uart == UART_3) {
    // UART3 configurations
    enable_UART3_power();
    set_DLAB_1_UART3();
    set_UART3_8bit();
    disable_fractional_div_UART3();
    set_DLM_DLL_UART3(baud_rate);
    disable_DLAB_1_UART3();
    // enable_FIFO_UART3();
  } else
    fprintf(stderr, "ERROR uart_lab__init: Please use UART2 or UART3 ONLY\n");
}

bool uart_lab__polled_get(uart_number_e uart, char *input_byte) {

  if (uart == UART_2) {
    UART_RDR_wait(uart);
    *input_byte = LPC_UART2->RBR;
  } else if (uart == UART_3) {
    UART_RDR_wait(uart);
    *input_byte = LPC_UART3->RBR;
  } else
    fprintf(stderr, "ERROR uart_lab__polled_get: Please use UART2 or UART3 ONLY\n");

  return 1;
}

bool uart_lab__polled_put(uart_number_e uart, char output_byte) {

  if (uart == UART_2) {
    UART_THRE_wait(uart);
    LPC_UART2->THR = output_byte;
  } else if (uart == UART_3) {
    UART_THRE_wait(uart);
    LPC_UART3->THR = output_byte;
  } else
    fprintf(stderr, "ERROR uart_lab__polled_out: Please use UART2 or UART3 ONLY\n");

  return 1;
}

void UART_RDR_wait(uart_number_e uart) {
  const uint32_t RDR_pin = (1 << 0);

  if (uart == UART_2) {
    while (!(LPC_UART2->LSR & RDR_pin)) {
    } // Empty while statement
  } else if (uart == UART_3) {
    while (!(LPC_UART3->LSR & RDR_pin)) {
    } // Empty while statement
  } else
    fprintf(stderr, "ERROR UART_RDR_wait: Please use UART2 or UART3 ONLY\n");
}

void UART_THRE_wait(uart_number_e uart) {
  const uint32_t THRE_pin = (1 << 5);

  if (uart == UART_2) {
    while (!(LPC_UART2->LSR & THRE_pin)) {
    } // Empty while statement
  } else if (uart == UART_3) {
    while (!(LPC_UART3->LSR & THRE_pin)) {
    } // Empty while statement
  } else
    fprintf(stderr, "ERROR UART_THRE_wait: Please use UART2 or UART3 ONLY\n");
}

void enable_UART2_power() {
  const uint32_t PCUART_2_pin = (1 << 24);
  LPC_SC->PCONP |= PCUART_2_pin;
}

void enable_UART3_power() {
  const uint32_t PCUART_3_pin = (1 << 25);
  LPC_SC->PCONP |= PCUART_3_pin;
}

void set_DLAB_1_UART2() {
  const uint32_t DLAB_UART2_pin = (1 << 7);
  LPC_UART2->LCR = DLAB_UART2_pin;
}

void set_DLAB_1_UART3() {
  const uint32_t DLAB_UART3_pin = (1 << 7);
  LPC_UART3->LCR = DLAB_UART3_pin;
}

void disable_DLAB_1_UART2() {
  const uint32_t DLAB_UART2_pin = (1 << 7);
  LPC_UART2->LCR &= ~DLAB_UART2_pin;
}

void disable_DLAB_1_UART3() {
  const uint32_t DLAB_UART3_pin = (1 << 7);
  LPC_UART3->LCR &= ~DLAB_UART3_pin;
}

void disable_fractional_div_UART2() {
  const uint32_t divaddval = (0 << 0);
  const uint32_t mulval = (1 << 4);
  LPC_UART2->FDR |= (divaddval | mulval);
}

void disable_fractional_div_UART3() {
  const uint32_t divaddval = (0 << 0);
  const uint32_t mulval = (1 << 4);
  LPC_UART3->FDR |= (divaddval | mulval);
}

void set_DLM_DLL_UART2(uint32_t baud_rate) {
  const uint16_t value_16bit = (96U * 1000 * 1000) / (16 * baud_rate);
  LPC_UART2->DLL = (value_16bit & 0xFF);
  LPC_UART2->DLM = (value_16bit >> 8) & 0xFF;
}

void set_DLM_DLL_UART3(uint32_t baud_rate) {
  const uint16_t value_16bit = (96U * 1000 * 1000) / (16 * baud_rate);
  LPC_UART3->DLL = (value_16bit & 0xFF);
  LPC_UART3->DLM = (value_16bit >> 8) & 0xFF;
}

void set_UART2_8bit() {
  const uint32_t WLS_pins = 0x3;
  LPC_UART2->LCR |= WLS_pins;
}

void set_UART3_8bit() {
  const uint32_t WLS_pins = 0x3;
  LPC_UART3->LCR |= WLS_pins;
}

void enable_FIFO_UART2() {
  uint32_t FIFO_pin = (1 << 0);
  LPC_UART2->FCR |= FIFO_pin;
}

void enable_FIFO_UART3() {
  uint32_t FIFO_pin = (1 << 0);
  LPC_UART3->FCR |= FIFO_pin;
}