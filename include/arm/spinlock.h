#ifndef ARM_SPINLOCK_H
#define ARM_SPINLOCK_H 1

#include <stdint.h>

#include "hal/spinlock.h"

#define SPINLOCK() (spinlock){0}

typedef struct spinlock {
  uint32_t owner;
  uint32_t next;
  uint32_t irq;
} spinlock;

static inline spinlock spinlockCreate(void) { return SPINLOCK(); }

#endif
