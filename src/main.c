#include <stdint.h>

#include "f446xx/gpio.h"
#include "f446xx/rcc.h"

#include "hal/clock.h"

static volatile uint32_t *rcc_apb2enr =
    (volatile uint32_t *)(RCC_BASE + RCC_APB2ENR);

static volatile uint16_t *spi_cr1 = (volatile uint16_t *)0x40013000;
static volatile uint16_t *spi_sr = (volatile uint16_t *)0x40013008;
static volatile uint8_t *spi_dr = (volatile uint8_t *)0x4001300C;

const gpio MOSI = GPIO_PA7;
const gpio SCLK = GPIO_PA5;
const gpio CS = GPIO_PA15;
const gpio DC = GPIO_PA10;
const gpio RST = GPIO_PC9;

const uint8_t gamma_lut[] = {
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
    0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x15, 0x17, 0x19, 0x1B,
    0x1D, 0x1F, 0x21, 0x23, 0x25, 0x27, 0x2A, 0x2D, 0x30, 0x33, 0x36,
    0x39, 0x3C, 0x3F, 0x42, 0x45, 0x48, 0x4C, 0x50, 0x54, 0x58, 0x5C,
    0x60, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7D, 0x82, 0x87, 0x8C,
    0x91, 0x96, 0x9B, 0xA0, 0xA5, 0xAA, 0xAF, 0xB4,
};

void spiWriteRaw(uint8_t b) {
  *spi_dr = b; // send command
  while ((!(*spi_sr & (0b1u << 1))) ||
         (*spi_sr & ((0b1u << 7)))) // wait for Tx to complete
    ;
}

void spiWrite(uint8_t b) {
  gpioWrite(CS, 0);
  spiWriteRaw(b);
  gpioWrite(CS, 1);
}

void spiCommand(uint8_t b) {
  gpioWrite(DC, 0);
  spiWrite(b);
}

void spiData(uint8_t b) {
  gpioWrite(DC, 1);
  spiWrite(b);
}

int main(void) {
  clockEnable();

  gpioEnable(MOSI);
  gpioEnable(SCLK);
  gpioEnable(CS);
  gpioEnable(DC);
  gpioEnable(RST);

  gpioMode(MOSI, GPIO_MODE_ALT);
  gpioMode(SCLK, GPIO_MODE_ALT);
  gpioMode(CS, GPIO_MODE_OUTPUT);
  gpioMode(DC, GPIO_MODE_OUTPUT);
  gpioMode(RST, GPIO_MODE_OUTPUT);

  gpioAlt(MOSI, GPIO_AF5); // configure to SPI
  gpioAlt(SCLK, GPIO_AF5); // configure to SPI

  gpioWrite(RST, 0); // pull RST low to power cycle the display
  sleep(20);
  gpioWrite(RST, 1);
  sleep(20);

  *rcc_apb2enr |= (0b1u << 12); // enable SPI1 peripheral clock
  sleep(5);

  *spi_cr1 = 0b0000001101001100;
  sleep(5);

  spiCommand(0xFD);
  spiData(0x12);

  spiCommand(0xFD);
  spiData(0xB1);

  spiCommand(0xAE);

  spiCommand(0xB3);
  spiData(0xF1);

  spiCommand(0xCA);
  spiData(0x7F);

  spiCommand(0xA2);
  spiData(0x00);

  spiCommand(0xA1);
  spiData(0x00);

  spiCommand(0xA0);
  spiData(0b00100100);

  spiCommand(0xB5);
  spiData(0x00);

  spiCommand(0xAB);
  spiData(0x1);

  spiCommand(0xB4);
  spiData(0xA0);
  spiData(0xB5);
  spiData(0x55);

  spiCommand(0xC1);
  spiData(0xC8);
  spiData(0x80);
  spiData(0xC8);

  spiCommand(0xC7);
  spiData(0x0F);

  spiCommand(0xB8);
  for (int i = 0; i < 63; i++)
    spiData(gamma_lut[i]);

  spiCommand(0xB1);
  spiData(0x32);

  spiCommand(0xB2);
  spiData(0xA4);
  spiData(0x00);
  spiData(0x00);

  spiCommand(0xBB);
  spiData(0x17);

  spiCommand(0xB6);
  spiData(0x01);

  spiCommand(0xBE);
  spiData(0x05);

  spiCommand(0xA6);

  spiCommand(0xAF);

  sleep(5);

  spiCommand(0x5C);
  *spi_cr1 &= ~(1u << 6); // disable SPI
  *spi_cr1 |= (1u << 11); // use 16-bit frames
  *spi_cr1 |= (1u << 6);  // enable SPI

  gpioWrite(DC, 1);

  extern uint16_t frames_START;
  extern uint16_t frames_END;
  uint32_t frame_SIZE = 128 * 128;
  uint32_t frame_COUNT = (uint32_t)(&frames_END - &frames_START) / frame_SIZE;

  uint32_t frame = 0;
  for (;;) {
    uint16_t *fp = &frames_START + frame * frame_SIZE;

    gpioWrite(CS, 0);
    for (int i = 0; i < frame_SIZE; i++) {
      *((volatile uint16_t *)spi_dr) = fp[i]; // send command
      while (*spi_sr & ((0b1u << 7)))         // wait for Tx to complete
        ;
    }
    gpioWrite(CS, 1);

    if (++frame >= frame_COUNT)
      frame = 0;
  }

  return 0;
}