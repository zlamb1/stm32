#ifndef HAL_SPINLOCK_H
#define HAL_SPINLOCK_H 1

#include <stdbool.h>

typedef struct spinlock spinlock;

/**
 * @brief Attempt to lock a spinlock.
 * @note Can spuriously fail.
 * @param sp The spinlock to lock.
 */
bool spinTryLock(spinlock *sp);

/**
 * @brief Wait on and lock a spinlock.
 * @param sp The spinlock to lock.
 */
void spinLock(spinlock *sp);

/**
 * @brief Unlock a spinlock.
 * @note The spinlock must have previously been
 * successfully locked or the behavior is undefined.
 * @param sp The spinlock to unlock.
 */
void spinUnlock(spinlock *sp);

#endif