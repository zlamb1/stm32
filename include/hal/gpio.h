#ifndef HAL_GPIO_H
#define HAL_GPIO_H 1

typedef struct gpio gpio;

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