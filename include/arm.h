#ifndef ARM_H
#define ARM_H 1

static inline void dsb(void) { __asm__ volatile("dsb" ::: "memory"); }

#endif