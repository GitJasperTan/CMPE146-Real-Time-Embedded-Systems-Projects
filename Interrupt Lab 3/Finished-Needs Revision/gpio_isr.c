// @file gpio_isr.c
#include "gpio_isr.h"

// Note: You may want another separate array for falling vs. rising edge callbacks
static function_pointer_t gpio0_callbacks[32];

void gpio0__attach_interrupt(uint32_t pin, gpio_interrupt_e interrupt_type, function_pointer_t callback) {
  // 1) Store the callback based on the pin at gpio0_callbacks
  // 2) Configure GPIO 0 pin for rising or falling edge

  gpio0_callbacks[pin] = callback;

  if (interrupt_type == GPIO_INTR__RISING_EDGE) {
    LPC_GPIOINT->IO0IntEnR |= (1 << pin);
  } else {
    LPC_GPIOINT->IO0IntEnF |= (1 << pin);
  }

  fprintf(stderr, "Check 1\n");
}

// We wrote some of the implementation for you

void gpio0__interrupt_dispatcher(void) {
  // Check which pin generated the interrupt

  // NVIC_EnableIRQ(GPIO_IRQn);

  int pin_that_generated_interrupt = logic_that_you_will_write();
  function_pointer_t attached_user_handler = gpio0_callbacks[pin_that_generated_interrupt];
  attached_user_handler();
  clear_pin_interrupt(pin_that_generated_interrupt);
}

int logic_that_you_will_write() {

  for (int i = 0; i < 32; i = i + 1) {
    if (LPC_GPIOINT->IO0IntStatF & (1 << i))
      return i;
    if (LPC_GPIOINT->IO0IntStatR & (1 << i))
      return i;
  }
  return 0;
}

void clear_pin_interrupt(int pin) { LPC_GPIOINT->IO0IntClr |= (1 << pin); }