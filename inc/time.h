#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>


volatile uint32_t msTicks;

void tick(void);
void delay(uint32_t num_ticks);

#endif // _TIME_H_