#ifndef CORTEX_M4_CLOCK_H
#define CORTEX_M4_CLOCK_H 1

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Configure the system timer.
 * @param enable Enables the system timer.
 * @param tickint Enables SysTick exception request.
 * @param clksource Indicates the clock source. 0 for external clock and 1 for
 * processor clock.
 */
void sysTickConfigure(bool enable, bool tickint, bool clksource);

/**
 * @brief Queries whether COUNTFLAG is set.
 * @return COUNTFLAG: Indicates whether the system timer has ticked to
 * zero since the last restart or read of COUNTFLAG.
 */
bool sysTickCountFlag(void);

/**
 * @brief Set the reload value for the system timer. This value
 * will be reloaded into the system timer current value when it reaches 0.
 * @param reload The value to be reloaded. Only low 24-bits are valid.
 */
void sysTickReload(uint32_t reload);

/**
 * @return The current value of the system timer.
 */
uint32_t sysTickCurrent(void);

/**
 * @brief Clears current value to zero. Additionally clears the COUNTFLAG.
 */
void sysTickRestart(void);

/**
 * @note This is a calibration hint. Clock skew and other factors can render it
 * inaccurate.
 * @return The reload value for 10ms (100Hz) of timing. If the value is 0,
 * it is not applicable.
 */
uint32_t sysTickCalibration(void);

#endif