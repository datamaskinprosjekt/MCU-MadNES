#include "sys_time.h"

/**************************************************************
 * Interrupt Service Routine for system tick counter.
 *************************************************************/
void SysTick_Handler(void)
{
  ticks++;
}


/**************************************************************
 * Delays a number of ticks.
 * @param num_ticks Number of ticks to delay
 *************************************************************/
void delay(uint32_t num_ticks)
{
  uint32_t current_ticks;
  
  current_ticks = ticks;

  while ((ticks - current_ticks) < num_ticks) ;
}