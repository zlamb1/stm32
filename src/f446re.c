#include "f446re.h"

static volatile uint16_t gpio_enabled[GPIO_PORT_MAX + 1];

static inline volatile gpio_reg *gpio_get_reg(gpio_port gp) {
  return (volatile gpio_reg *)(GPIO_BASE + (uintptr_t)gp * 0x400);
}

void gpio_enable(gpio g) {
  if (!gpio_enabled[g.port]) {
    volatile uint32_t *rcc_ahb1enr =
        (volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR);
    uint32_t v = *rcc_ahb1enr;
    v |= (0b1u << g.port);
    *rcc_ahb1enr = v;
  }
  gpio_enabled[g.port] |= (0b1u << g.pin);
}

void gpio_disable(gpio g) {
  gpio_enabled[g.port] &= ~(0b1u << g.pin);
  if (!gpio_enabled[g.port]) {
    volatile uint32_t *rcc_ahb1enr =
        (volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR);
    uint32_t v = *rcc_ahb1enr;
    v &= ~(0b1u << g.port);
    *rcc_ahb1enr = v;
  }
}

int gpio_get_enabled(gpio g) { return (gpio_enabled[g.port] >> g.pin) & 0b1; }

gpio_mode gpio_get_mode(gpio g) {
  __auto_type gr = gpio_get_reg(g.port);
  return (gr->mode >> (g.pin * 2)) & 0b11;
}

uint8_t gpio_input(gpio g) {
  __auto_type gr = gpio_get_reg(g.port);
  return (gr->idr >> g.pin) & 0b1;
}

void gpio_output(gpio g, uint8_t v) {
  __auto_type gr = gpio_get_reg(g.port);
  uint32_t o = gr->odr;
  o &= ~(0b1u << g.pin);
  o |= ((v & 0b1) << g.pin);
  gr->odr = o;
}

void gpio_set_mode(gpio g, gpio_mode mode) {
  __auto_type gr = gpio_get_reg(g.port);
  uint32_t m = gr->mode;
  m &= ~(0b11u << (g.pin * 2));
  m |= ((mode & 0b11) << (g.pin * 2));
  gr->mode = m;
}

void gpio_set_alt(gpio g, uint8_t alt) {
  __auto_type gr = gpio_get_reg(g.port);
  if (g.pin < 8) {
    gr->afrl =
        (gr->afrl & ~(0b1111u << (g.pin * 4))) | ((uint32_t)alt << (g.pin * 4));
  } else {
    gr->afrh = (gr->afrh & ~(0b1111u << ((g.pin - 8) * 4))) |
               ((uint32_t)alt << ((g.pin - 8) * 4));
  }
}

void systick_enable(void) {
  volatile uint32_t *stcsr = (volatile uint32_t *)0xE000E010;
  volatile uint32_t *strvr = (volatile uint32_t *)0xE000E014;
  volatile uint32_t *stcvr = (volatile uint32_t *)0xE000E018;
  *strvr = 0;
  *stcvr = 0;
  *stcsr = (*stcsr & ~0b111) | 0b001;
}

void sleep(uint16_t ms) {
  volatile uint32_t *stcsr = (volatile uint32_t *)0xE000E010;
  volatile uint32_t *strvr = (volatile uint32_t *)0xE000E014;
  volatile uint32_t *stcvr = (volatile uint32_t *)0xE000E018;
  *strvr = (uint32_t)ms * 18750;
  *stcvr = 0;
  for (;;) {
    if (*stcsr & (0b1u << 16))
      break;
  }
  __asm__ volatile("" ::: "memory"); // prevent reordering
}