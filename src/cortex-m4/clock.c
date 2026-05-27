#include <stdint.h>

#include "cortex-m4/clock.h"

#include "hal/bits.h"

volatile uint32_t *const SYST_CSR = (volatile uint32_t *)0xE000E010;
volatile uint32_t *const SYST_RVR = (volatile uint32_t *)0xE000E014;
volatile uint32_t *const SYST_CVR = (volatile uint32_t *)0xE000E018;
volatile uint32_t *const SYST_CALIB = (volatile uint32_t *)0xE000E01C;

void sysTickConfigure(bool enable, bool tickint, bool clksource) {
  uint32_t v = 0;
  BIT_WRITE(v, 0, enable);
  BIT_WRITE(v, 1, tickint);
  BIT_WRITE(v, 2, clksource);
  *SYST_CSR = v;
}

bool sysTickCountFlag(void) { return !!BIT_READ(*SYST_CSR, 16); }

void sysTickReload(uint32_t reload) {
  reload &= 0x00FFFFFF;
  *SYST_RVR = reload;
}

uint32_t sysTickCurrent(void) { return *SYST_CVR & 0x00FFFFFF; }

void sysTickRestart(void) { *SYST_CVR = 0; }

uint32_t sysTickCalibration(void) { return *SYST_CALIB & 0x00FFFFFF; }