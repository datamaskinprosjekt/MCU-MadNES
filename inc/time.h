#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

static volatile uint32_t msTicks;

void SysTick_Handler(void);

void Delay(uint32_t dlyTicks);

#endif