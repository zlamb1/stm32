#include <stdint.h>

#include "cortex-m4/clock.h"

void clockEnable(void) { sysTickConfigure(true, false, false); }

void sleep(unsigned ms) {
  // Note: sysTickCalibration should return a sensible value.
  sysTickReload((uint32_t)ms * sysTickCalibration());
  sysTickRestart();
  while (!sysTickCountFlag())
    ;
}