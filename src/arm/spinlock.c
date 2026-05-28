#include "arm/spinlock.h"

#include "hal/arm.h"

static inline void spinAcquire(spinlock *sp) { dmb(); }

bool spinTryLock(spinlock *sp) {
  uint32_t owner = ldr(&sp->owner);
  uint32_t ticket = ldrex(&sp->next);

  if (owner != ticket) {
    clrex();
    return false;
  }

  if (!strex(&sp->next, ticket + 1))
    return false;

  spinAcquire(sp);
  return true;
}

void spinLock(spinlock *sp) {
  uint32_t ticket;

  for (;;) {
    ticket = ldrex(&sp->next);
    if (strex(&sp->next, ticket + 1))
      break;
  }

  while (ldr(&sp->owner) != ticket)
    wfe();

  spinAcquire(sp);
}

void spinUnlock(spinlock *sp) {
  uint32_t owner;
  dmb();
  owner = ldr(&sp->owner);
  str(&sp->owner, owner + 1);
  dsb();
  sev();
}