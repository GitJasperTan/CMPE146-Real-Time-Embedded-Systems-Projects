#include "gpio_lab.h"
#include "lpc40xx.h"

void gpio0__set_as_input(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  switch (port_num) {
  case 0:
    LPC_GPIO0->DIR &= ~temp;
  case 1:
    LPC_GPIO1->DIR &= ~temp;
  case 2:
    LPC_GPIO2->DIR &= ~temp;
  case 3:
    LPC_GPIO3->DIR &= ~temp;
  case 4:
    LPC_GPIO4->DIR &= ~temp;
  case 5:
    LPC_GPIO5->DIR &= ~temp;
  }
}

void gpio0__set_as_output(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  switch (port_num) {
  case 0:
    LPC_GPIO0->DIR |= temp;
  case 1:
    LPC_GPIO1->DIR |= temp;
  case 2:
    LPC_GPIO2->DIR |= temp;
  case 3:
    LPC_GPIO3->DIR |= temp;
  case 4:
    LPC_GPIO4->DIR |= temp;
  case 5:
    LPC_GPIO5->DIR |= temp;
  }
}

void gpio0__set_high(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  switch (port_num) {
  case 0:
    LPC_GPIO0->PIN |= temp;
  case 1:
    LPC_GPIO1->PIN |= temp;
  case 2:
    LPC_GPIO2->PIN |= temp;
  case 3:
    LPC_GPIO3->PIN |= temp;
  case 4:
    LPC_GPIO4->PIN |= temp;
  case 5:
    LPC_GPIO5->PIN |= temp;
  }
}

void gpio0__set_low(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  switch (port_num) {
  case 0:
    LPC_GPIO0->PIN &= ~temp;
  case 1:
    LPC_GPIO1->PIN &= ~temp;
  case 2:
    LPC_GPIO2->PIN &= ~temp;
  case 3:
    LPC_GPIO3->PIN &= ~temp;
  case 4:
    LPC_GPIO4->PIN &= ~temp;
  case 5:
    LPC_GPIO5->PIN &= ~temp;
  }
}

void gpio0__set(uint8_t port_num, uint8_t pin_num, bool high) {
  uint32_t temp = (1 << pin_num);

  switch (port_num) {
  case 0:
    (high) ? (LPC_GPIO0->PIN |= temp) : (LPC_GPIO0->PIN &= ~temp);
  case 1:
    (high) ? (LPC_GPIO1->PIN |= temp) : (LPC_GPIO1->PIN &= ~temp);
  case 2:
    (high) ? (LPC_GPIO2->PIN |= temp) : (LPC_GPIO2->PIN &= ~temp);
  case 3:
    (high) ? (LPC_GPIO3->PIN |= temp) : (LPC_GPIO3->PIN &= ~temp);
  case 4:
    (high) ? (LPC_GPIO4->PIN |= temp) : (LPC_GPIO4->PIN &= ~temp);
  case 5:
    (high) ? (LPC_GPIO5->PIN |= temp) : (LPC_GPIO5->PIN &= ~temp);
  }
}

bool gpio0__get_level(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  switch (port_num) {
  case 0:
    return (LPC_GPIO0->PIN & temp);
  case 1:
    return (LPC_GPIO1->PIN & temp);
  case 2:
    return (LPC_GPIO2->PIN & temp);
  case 3:
    return (LPC_GPIO3->PIN & temp);
  case 4:
    return (LPC_GPIO4->PIN & temp);
  case 5:
    return (LPC_GPIO5->PIN & temp);
  default:
    return (0);
  }
}