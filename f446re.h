#ifndef F446RE_H
#define F446RE_H 1

#include <stdint.h>

#define GPIO(_port, _pin)                                                      \
  ({                                                                           \
    _Static_assert(_port <= GPIO_PORT_MAX, "invalid GPIO port");               \
    _Static_assert(_pin <= 15, "invalid GPIO pin [must be 1-15]");             \
    (gpio){.port = _port, .pin = _pin};                                        \
  })
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

#define RCC_BASE 0x40023800
#define RCC_AHB1ENR 0x30

typedef enum {
  GPIO_PORT_A = 0,
  GPIO_PORT_B = 1,
  GPIO_PORT_C = 2,
  GPIO_PORT_D = 3,
  GPIO_PORT_E = 4,
  GPIO_PORT_F = 5,
  GPIO_PORT_G = 6,
  GPIO_PORT_H = 7,
  GPIO_PORT_MAX = 7,
} gpio_port;

typedef struct {
  uint32_t mode;
  uint32_t otype;
  uint32_t ospeed;
  uint32_t updr;
  uint32_t idr;
  uint32_t odr;
  uint32_t bsrr;
  uint32_t lckr;
  uint32_t afrl;
  uint32_t afrh;
} gpio_reg;

typedef struct {
  gpio_port port;
  uint8_t pin;
} gpio;

typedef enum {
  GPIO_MODE_INPUT = 0b00,
  GPIO_MODE_OUTPUT = 0b01,
  GPIO_MODE_ALT = 0b10,
  GPIO_MODE_ANALOG = 0b11,
} gpio_mode;

void gpio_enable(gpio g);
void gpio_disable(gpio g);

int gpio_get_enabled(gpio g);
gpio_mode gpio_get_mode(gpio g);

uint8_t gpio_input(gpio g);
void gpio_output(gpio g, uint8_t v);
void gpio_set_mode(gpio g, gpio_mode mode);

#endif