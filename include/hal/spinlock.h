#ifndef HAL_SPINLOCK_H
#define HAL_SPINLOCK_H 1

#include <stdbool.h>

typedef struct spinlock spinlock;

bool spinTryLock(spinlock *);
void spinLock(spinlock *);
void spinUnlock(spinlock *);

#endif