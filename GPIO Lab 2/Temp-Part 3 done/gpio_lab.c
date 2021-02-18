#include "gpio_lab.h"
#include "lpc40xx.h"

void gpio0__set_as_input(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  //   if (port_num == 0)
  //     LPC_GPIO0->DIR &= ~temp;
  //   else if (port_num == 1)
  //     LPC_GPIO1->DIR &= ~temp;
  //   else if (port_num == 2)
  //     LPC_GPIO2->DIR &= ~temp;
  //   else if (port_num == 3)
  //     LPC_GPIO3->DIR &= ~temp;
  //   else if (port_num == 4)
  //     LPC_GPIO4->DIR &= ~temp;
  //   else if (port_num == 5)
  //     LPC_GPIO5->DIR &= ~temp;

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

  // LPC_GPIO1->DIR &= ~temp;
}

void gpio0__set_as_output(uint8_t port_num, uint8_t pin_num) {
  uint32_t temp = (1 << pin_num);

  //   if (port_num == 0)
  //     LPC_GPIO0->DIR |= temp;
  //   else if (port_num == 1)
  //     LPC_GPIO1->DIR |= temp;
  //   else if (port_num == 2)
  //     LPC_GPIO2->DIR |= temp;
  //   else if (port_num == 3)
  //     LPC_GPIO3->DIR |= temp;
  //   else if (port_num == 4)
  //     LPC_GPIO4->DIR |= temp;
  //   else if (port_num == 5)
  //     LPC_GPIO5->DIR |= temp;

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

  //   if (port_num == 0)
  //     LPC_GPIO0->PIN |= temp;
  //   else if (port_num == 1)
  //     LPC_GPIO1->PIN |= temp;
  //   else if (port_num == 2)
  //     LPC_GPIO2->PIN |= temp;
  //   else if (port_num == 3)
  //     LPC_GPIO3->PIN |= temp;
  //   else if (port_num == 4)
  //     LPC_GPIO4->PIN |= temp;
  //   else if (port_num == 5)
  //     LPC_GPIO5->PIN |= temp;

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

  //   if (port_num == 0)
  //     LPC_GPIO0->PIN &= ~temp;
  //   else if (port_num == 1)
  //     LPC_GPIO1->PIN &= ~temp;
  //   else if (port_num == 2)
  //     LPC_GPIO2->PIN &= ~temp;
  //   else if (port_num == 3)
  //     LPC_GPIO3->PIN &= ~temp;
  //   else if (port_num == 4)
  //     LPC_GPIO4->PIN &= ~temp;
  //   else if (port_num == 5)
  //     LPC_GPIO5->PIN &= ~temp;

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

  //   if (port_num == 0) {
  //     if (high)
  //       LPC_GPIO0->PIN |= temp;
  //     else
  //       LPC_GPIO0->PIN &= ~temp;
  //   } else if (port_num == 1) {
  //     if (high)
  //       LPC_GPIO1->PIN |= temp;
  //     else
  //       LPC_GPIO1->PIN &= ~temp;
  //   } else if (port_num == 2) {
  //     if (high)
  //       LPC_GPIO2->PIN |= temp;
  //     else
  //       LPC_GPIO2->PIN &= ~temp;
  //   } else if (port_num == 3) {
  //     if (high)
  //       LPC_GPIO3->PIN |= temp;
  //     else
  //       LPC_GPIO3->PIN &= ~temp;
  //   } else if (port_num == 4) {
  //     if (high)
  //       LPC_GPIO4->PIN |= temp;
  //     else
  //       LPC_GPIO4->PIN &= ~temp;
  //   } else if (port_num == 5) {
  //     if (high)
  //       LPC_GPIO5->PIN |= temp;
  //     else
  //       LPC_GPIO5->PIN &= ~temp;
  //   }
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

  //   if (port_num == 0)
  //     return (LPC_GPIO0->PIN & temp);
  //   else if (port_num == 1)
  //     return (LPC_GPIO1->PIN & temp);
  //   else if (port_num == 2)
  //     return (LPC_GPIO2->PIN & temp);
  //   else if (port_num == 3)
  //     return (LPC_GPIO3->PIN & temp);
  //   else if (port_num == 4)
  //     return (LPC_GPIO4->PIN & temp);
  //   else if (port_num == 5)
  //     return (LPC_GPIO5->PIN & temp);
  //   else
  //     return (0);
}
