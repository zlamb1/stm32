#include "f446re.h"

static volatile uint32_t *rcc_apb2enr =
    (volatile uint32_t *)(RCC_BASE + RCC_APB2ENR);

static volatile uint16_t *spi_cr1 = (volatile uint16_t *)0x40013000;
static volatile uint16_t *spi_sr = (volatile uint16_t *)0x40013008;
static volatile uint8_t *spi_dr = (volatile uint8_t *)0x4001300C;

gpio A7 = GPIO(GPIO_PORT_A, 7);   // DIN
gpio A5 = GPIO(GPIO_PORT_A, 5);   // CLK
gpio A15 = GPIO(GPIO_PORT_A, 15); // CS#
gpio A10 = GPIO(GPIO_PORT_A, 10); // D/C#
gpio C9 = GPIO(GPIO_PORT_C, 9);   // RST
gpio C13 = GPIO(GPIO_PORT_C, 13);

const uint8_t gamma_lut[] = {
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
    0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x15, 0x17, 0x19, 0x1B,
    0x1D, 0x1F, 0x21, 0x23, 0x25, 0x27, 0x2A, 0x2D, 0x30, 0x33, 0x36,
    0x39, 0x3C, 0x3F, 0x42, 0x45, 0x48, 0x4C, 0x50, 0x54, 0x58, 0x5C,
    0x60, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7D, 0x82, 0x87, 0x8C,
    0x91, 0x96, 0x9B, 0xA0, 0xA5, 0xAA, 0xAF, 0xB4,
};

void spi_tx_raw(uint8_t b) {
  *spi_dr = b; // send command
  while ((!(*spi_sr & (0b1u << 1))) ||
         (*spi_sr & ((0b1u << 7)))) // wait for Tx to complete
    ;
}

void spi_tx(uint8_t b) {
  gpio_output(A15, 0); // pull CS# low
  spi_tx_raw(b);
  gpio_output(A15, 1); // pull CS# high
}

void spi_cmd(uint8_t b) {
  gpio_output(A10, 0); // pull D/C# low to issue commands
  spi_tx(b);
}

void spi_data(uint8_t b) {
  gpio_output(A10, 1); // pull D/C# low to issue data
  spi_tx(b);
}

int main(void) {
  gpio A7 = GPIO(GPIO_PORT_A, 7);   // DIN
  gpio A5 = GPIO(GPIO_PORT_A, 5);   // CLK
  gpio A15 = GPIO(GPIO_PORT_A, 15); // CS#
  gpio A10 = GPIO(GPIO_PORT_A, 10); // D/C#
  gpio C9 = GPIO(GPIO_PORT_C, 9);   // RST
  gpio C13 = GPIO(GPIO_PORT_C, 13);

  uint16_t color;

  systick_enable();

  gpio_enable(A7);
  gpio_enable(A5);
  gpio_enable(A15);
  gpio_enable(A10);
  gpio_enable(C9);
  gpio_enable(C13);

  gpio_set_mode(A7, GPIO_MODE_ALT);
  gpio_set_mode(A5, GPIO_MODE_ALT);
  gpio_set_mode(A15, GPIO_MODE_OUTPUT);
  gpio_set_mode(A10, GPIO_MODE_OUTPUT);
  gpio_set_mode(C9, GPIO_MODE_OUTPUT);
  gpio_set_mode(C13, GPIO_MODE_INPUT);

  gpio_set_alt(A7, 5); // configure to SPI
  gpio_set_alt(A5, 5); // configure to SPI

  gpio_output(C9, 0); // pull RST low to power cycle the display
  sleep(20);
  gpio_output(C9, 1);
  sleep(20);

  *rcc_apb2enr |= (0b1u << 12); // enable SPI1 peripheral clock
  sleep(5);

  *spi_cr1 = 0b0000001101001100;
  sleep(5);

  spi_cmd(0xFD);
  spi_data(0x12);

  spi_cmd(0xFD);
  spi_data(0xB1);

  spi_cmd(0xAE);

  spi_cmd(0xB3);
  spi_data(0xF1);

  spi_cmd(0xCA);
  spi_data(0x7F);

  spi_cmd(0xA2);
  spi_data(0x00);

  spi_cmd(0xA1);
  spi_data(0x00);

  spi_cmd(0xA0);
  spi_data(0b00100100);

  spi_cmd(0xB5);
  spi_data(0x00);

  spi_cmd(0xAB);
  spi_data(0x1);

  spi_cmd(0xB4);
  spi_data(0xA0);
  spi_data(0xB5);
  spi_data(0x55);

  spi_cmd(0xC1);
  spi_data(0xC8);
  spi_data(0x80);
  spi_data(0xC8);

  spi_cmd(0xC7);
  spi_data(0x0F);

  spi_cmd(0xB8);
  for (int i = 0; i < 63; i++)
    spi_data(gamma_lut[i]);

  spi_cmd(0xB1);
  spi_data(0x32);

  spi_cmd(0xB2);
  spi_data(0xA4);
  spi_data(0x00);
  spi_data(0x00);

  spi_cmd(0xBB);
  spi_data(0x17);

  spi_cmd(0xB6);
  spi_data(0x01);

  spi_cmd(0xBE);
  spi_data(0x05);

  spi_cmd(0xA6);

  spi_cmd(0xAF);

  sleep(5);

  spi_cmd(0x5C);
  *spi_cr1 &= ~(1u << 6); // disable SPI
  *spi_cr1 |= (1u << 11); // use 16-bit frames
  *spi_cr1 |= (1u << 6);  // enable SPI

  gpio_output(A10, 1);

  extern uint16_t frames_START;
  extern uint16_t frames_END;
  uint32_t frame_SIZE = 128 * 128;
  uint32_t frame_COUNT = (uint32_t)(&frames_END - &frames_START) / frame_SIZE;

  uint32_t frame = 0;
  for (;;) {
    uint16_t *fp = &frames_START + frame * frame_SIZE;

    gpio_output(A15, 0); // pull CS# low
    for (int i = 0; i < frame_SIZE; i++) {
      *((volatile uint16_t *)spi_dr) = fp[i]; // send command
      while (*spi_sr & ((0b1u << 7)))         // wait for Tx to complete
        ;
    }
    gpio_output(A15, 1); // pull CS# high

    if (++frame >= frame_COUNT)
      frame = 0;
  }

  return 0;
}