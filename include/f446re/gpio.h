#ifndef F446RE_GPIO_H
#define F446RE_GPIO_H 1

#define GPIO_PORTS(X)                                                          \
  X(A, 0)                                                                      \
  X(B, 1)                                                                      \
  X(C, 2)                                                                      \
  X(D, 3)                                                                      \
  X(E, 4)                                                                      \
  X(F, 5)                                                                      \
  X(G, 6)                                                                      \
  X(H, 7)

#define GPIO_PINS(X, CTX)                                                      \
  X(0, CTX)                                                                    \
  X(1, CTX)                                                                    \
  X(2, CTX)                                                                    \
  X(3, CTX)                                                                    \
  X(4, CTX)                                                                    \
  X(5, CTX)                                                                    \
  X(6, CTX)                                                                    \
  X(7, CTX)                                                                    \
  X(8, CTX)                                                                    \
  X(9, CTX)                                                                    \
  X(10, CTX)                                                                   \
  X(11, CTX)                                                                   \
  X(12, CTX)                                                                   \
  X(13, CTX)                                                                   \
  X(14, CTX)                                                                   \
  X(15, CTX)

typedef enum {
#define X(NAME, N) GPIO_PORT_##NAME = N,
  GPIO_PORTS(X)
#undef X
      GPIO_PORT_COUNT,
} gpio_port;

typedef enum {
#define X(N, CTX) GPIO_PIN_##N,
  GPIO_PINS(X, 0)
#undef X
      GPIO_PIN_COUNT,
} gpio_pin;

typedef struct {
  gpio_port port;
  gpio_pin pin;
} gpio;

#define X(NAME, N) GPIO_PINS(Y, NAME)
#define Y(PIN, NAME)                                                           \
  static const gpio GPIO_P##NAME##PIN = {.port = GPIO_PORT_##NAME, .pin = PIN};
GPIO_PORTS(X)
#undef Y
#undef X

typedef enum {
  GPIO_MODE_INPUT,
  GPIO_MODE_OUTPUT,
  GPIO_MODE_ALT,
} gpio_mode;

/**
 * @brief Enable a GPIO pin for usage.
 * @param g The GPIO pin to enable.
 */
void gpioEnable(gpio g);

/**
 * @brief Disable a GPIO pin for usage.
 * @param g The GPIO pin to disable.
 */
void gpioDisable(gpio g);

/**
 * @brief Set the mode of a GPIO pin.
 * @param g The GPIO pin to configure.
 * @param m The new operational mode.
 */
void gpioMode(gpio g, gpio_mode m);

/**
 * @brief Write a digital state to a GPIO pin.
 * @param g The GPIO pin to write.
 * @param d The state to write. 0 for LOW otherwise HIGH.
 */
void gpioWrite(gpio g, unsigned int d);

/**
 * @brief Read the digital state of a GPIO pin.
 * @param g The GPIO pin to read.
 * @return The digital state of the pin. 0 for LOW or 1 for HIGH.
 */
unsigned int gpioRead(gpio g);

/**
 * @brief Set the GPIO pin's alternate function.
 * @param g The GPIO pin to configure.
 * @param alt The alternate function to use.
 */
void gpioAlt(gpio g, unsigned alt);

#endif