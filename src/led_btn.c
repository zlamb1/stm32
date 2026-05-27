#include "f446re/gpio.h"

int main(void) {
  gpioEnable(GPIO_PA5);
  gpioMode(GPIO_PA5, GPIO_MODE_OUTPUT);

  gpioEnable(GPIO_PC13);
  gpioMode(GPIO_PC13, GPIO_MODE_INPUT);

  for (;;) {
    gpioWrite(GPIO_PA5, !gpioRead(GPIO_PC13));
  }

  return 0;
}