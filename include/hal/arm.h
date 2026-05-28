#ifndef HAL_ARM_H
#define HAL_ARM_H 1

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Disables normal interrupts.
 */
static inline void cli(void) { __asm__ volatile("cpsid i" ::: "memory"); }

/**
 * @brief Clears the exclusive monitor state established
 * by ldrex.
 */
static inline void clrex(void) { __asm__ volatile("clrex" ::: "memory"); }

/**
 * @brief Data memory barrier. All explicit memory accesses prior to
 * this instruction must be observable before any later instructions.
 */
static inline void dmb(void) { __asm__ volatile("dmb" ::: "memory"); }

/**
 * @brief Data synchronization barrier. No instruction in program order after
 * this completes until this instruction completes.
 * @note All prior memory accesses, including cache, branch prediction,
 * and TLB maintenance finish before this instruction completes.
 */
static inline void dsb(void) { __asm__ volatile("dsb" ::: "memory"); }

/**
 * @brief Instruction synchronization barrier. Flushes the processor
 * pipeline before completing.
 * @note All instructions after this one are fetched from either cache
 * or memory.
 */
static inline void isb(void) { __asm__ volatile("isb" ::: "memory"); }

/**
 * Loads a value from a memory location.
 * @param p The memory location to load from.
 * @return The value stored at the memory location.
 */
static inline uint32_t ldr(const uint32_t *p) {
  uint32_t r;
  __asm__ volatile("ldr %0, [%1]" : "=r"(r) : "r"(p) : "memory");
  return r;
}

/**
 * @brief Starts an exclusive transaction while fetching
 * the value of a pointer.
 * @param p The pointer to fetch and acquire exclusively.
 * @return The value fetched.
 */
static inline uint32_t ldrex(const uint32_t *p) {
  uint32_t r;
  __asm__ volatile("ldrex %0, [%1]" : "=r"(r) : "r"(p) : "memory");
  return r;
}

/**
 * @brief Stores a value into a memory location.
 * @param p The memory location to store into.
 * @param v The value to store.
 */
static inline void str(uint32_t *p, uint32_t v) {
  __asm__ volatile("str %0, [%1]" ::"r"(v), "r"(p) : "memory");
}

/**
 * @brief Send an event to other processors to wake them up.
 */
static inline void sev(void) { __asm__ volatile("sev" ::: "memory"); }

/**
 * @brief Attempts an exclusive store on an address.
 * @param p The address to store to.
 * @param v The new value.
 * @return True if successful, false otherwise.
 */
static inline bool strex(uint32_t *p, uint32_t v) {
  uint32_t s;
  __asm__ volatile("strex %0, %1, [%2]" : "=&r"(s) : "r"(v), "r"(p) : "memory");
  return !s;
}

/**
 * @brief Wait for an event.
 */
static inline void wfe(void) { __asm__ volatile("wfe" ::: "memory"); }

/**
 * @brief Wait for an interrupt.
 */
static inline void wfi(void) { __asm__ volatile("wfi" ::: "memory"); }

#endif