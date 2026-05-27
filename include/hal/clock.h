#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H 1

/**
 * @brief Initializes any platform clocks necessary for sleep.
 */
void clockEnable(void);

/**
 * @brief Sleeps for some number of milliseconds.
 * @param ms The number of milliseconds to sleep for.
 */
void sleep(unsigned ms);

#endif