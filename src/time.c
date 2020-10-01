#include "time.h"

/* Interrupt Service Routine for system tick counter */
void SysTick_Handler(void)
{
  msTicks++;    // increment counter necessary in Delay()
}

/* Delay dlyTicks milliseconds */
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}