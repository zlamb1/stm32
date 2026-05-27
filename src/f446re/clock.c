#include <stdint.h>

#include "f446re/clock.h"

void sysTickEnable(void) {
  volatile uint32_t *stcsr = (volatile uint32_t *)0xE000E010;
  volatile uint32_t *strvr = (volatile uint32_t *)0xE000E014;
  volatile uint32_t *stcvr = (volatile uint32_t *)0xE000E018;
  *strvr = 0;
  *stcvr = 0;
  *stcsr = (*stcsr & ~0b111) | 0b001;
}

void sleep(unsigned ms) {
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