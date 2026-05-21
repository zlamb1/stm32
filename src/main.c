#include "f446re.h"

int main(void) {
  gpio A5 = GPIO(GPIO_PORT_A, 5);
  gpio C13 = GPIO(GPIO_PORT_C, 13);

  gpio_enable(A5);
  gpio_set_mode(A5, GPIO_MODE_OUTPUT);

  gpio_enable(C13);
  gpio_set_mode(C13, GPIO_MODE_INPUT);

  for (;;) {
    gpio_output(A5, !gpio_input(C13));
  }

  return 0;
}