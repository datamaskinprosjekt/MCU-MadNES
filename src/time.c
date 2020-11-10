#include "time.h"

void SysTick_Handler(void) {
  msTicks++;
}

void Delay(uint32_t dlyTicks) {
  uint32_t curTicks;
  
  curTicks = msTicks;

  while ((msTicks - curTicks) < dlyTicks) ;
}