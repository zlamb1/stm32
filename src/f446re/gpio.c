#include <stdint.h>

#include "f446xx/gpio.h"
#include "f446xx/rcc.h"

#include "hal/arm.h"
#include "hal/bits.h"
#include "hal/gpio.h"

#define GPIO_BASE 0x40020000
#define GPIO_OTYPE_PUSH_PULL 0
#define GPIO_OTYPE_OPEN_DRAIN 1
#define GPIO_OSPEED_LOW 0b00
#define GPIO_OSPEED_MED 0b01
#define GPIO_OSPEED_FAST 0b10
#define GPIO_OSPEED_HIGH 0b11
#define GPIO_PUPD_NONE 0b00
#define GPIO_PUPD_PULL_UP 0b01
#define GPIO_PUPD_PULL_DOWN 0b10
#define GPIO_AF(n) ((n) & 0xF)

typedef struct {
  uint32_t mode;
  uint32_t otype;
  uint32_t ospeed;
  uint32_t pupd;
  uint32_t id;
  uint32_t od;
  uint32_t bsr;
  uint32_t lck;
  uint32_t afl;
  uint32_t afh;
} gpio_reg;

static uint16_t ports[GPIO_PORT_COUNT];

static volatile gpio_reg *gpioReg(const gpio g) {
  return (volatile gpio_reg *)(GPIO_BASE + (uintptr_t)g.port * 0x400);
}

void gpioEnable(gpio g) {
  if (!ports[g.port]) {
    volatile uint32_t *rcc_ahb1enr =
        (volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR);
    BIT_SET(*rcc_ahb1enr, g.port);
    dsb();
  }
  BIT_SET(ports[g.port], g.pin);
}

void gpioDisable(gpio g) {
  BIT_CLEAR(ports[g.port], g.pin);
  if (!ports[g.port]) {
    volatile uint32_t *rcc_ahb1enr =
        (volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR);
    BIT_CLEAR(*rcc_ahb1enr, g.port);
  }
}

void gpioMode(gpio g, gpio_mode m) {
  volatile gpio_reg *reg = gpioReg(g);
  unsigned v;
  switch (m) {
  case GPIO_MODE_INPUT:
    v = 0x0;
    break;
  case GPIO_MODE_OUTPUT:
    v = 0x1;
    break;
  case GPIO_MODE_ALT:
    v = 0x2;
    break;
  }
  BITS_WRITE(reg->mode, g.pin * 2, 0x3, v);
}

void gpioWrite(gpio g, unsigned int d) {
  volatile gpio_reg *reg = gpioReg(g);
  BIT_WRITE(reg->od, g.pin, d);
}

unsigned int gpioRead(gpio g) {
  volatile gpio_reg *reg = gpioReg(g);
  return BIT_READ(reg->id, g.pin);
}

void gpioAlt(gpio g, gpio_alt alt) {
  volatile gpio_reg *reg = gpioReg(g);
  if (g.pin < 8)
    BITS_WRITE(reg->afl, g.pin * 4, 0xF, alt);
  else
    BITS_WRITE(reg->afh, (g.pin - 8) * 4, 0xF, alt);
}