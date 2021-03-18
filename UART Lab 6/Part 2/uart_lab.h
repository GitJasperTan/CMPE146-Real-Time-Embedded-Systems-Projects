#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include <stdbool.h>
#include <stdint.h>

static QueueHandle_t your_uart_rx_queue;

typedef enum {
  UART_2,
  UART_3,
} uart_number_e;

void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate);
// Refer to LPC User manual and setup the register bits correctly
// The first page of the UART chapter has good instructions
// a) Power on Peripheral
// b) Setup DLL, DLM, FDR, LCR registers

// Read the byte from RBR and actually save it to the pointer
bool uart_lab__polled_get(uart_number_e uart, char *input_byte);
// a) Check LSR for Receive Data Ready
// b) Copy data from RBR register to input_byte

bool uart_lab__polled_put(uart_number_e uart, char output_byte);
// a) Check LSR for Transmit Hold Register Empty
// b) Copy output_byte to THR register

void enable_UART2_power();
void enable_UART3_power();

void set_DLAB_1_UART2();
void set_DLAB_1_UART3();

void disable_DLAB_1_UART2();
void disable_DLAB_1_UART3();

void disable_DLAB_1_UART2();
void disable_DLAB_1_UART3();

void set_DLM_DLL_UART2(uint32_t baud_rate);
void set_DLM_DLL_UART3(uint32_t baud_rate);

void set_UART2_8bit();
void set_UART3_8bit();

void UART_THRE_wait(uart_number_e uart);
void UART_RDR_wait(uart_number_e uart);

void enable_FIFO_UART2();
void enable_FIFO_UART3();

// Task 2
static void your_receive_interrupt(void);
void uart__enable_receive_interrupt(uart_number_e uart_number);
bool uart_lab__get_char_from_queue(char *input_byte, uint32_t timeout);

void enable_interrupt_UART2();
void enable_interrupt_UART3();