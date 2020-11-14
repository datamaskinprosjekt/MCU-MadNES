#ifndef _INTERRUPT_HANDLERS_H_
#define _INTERRUPT_HANDLERS_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_gpio.h"
#include "ebi.h"
#include "fpga.h"
#include "object.h"
#include "game_logic.h"
#include "meta_data.h"


// NOTE: SDK Method to interface with ISER?
#define ISER0 ((volatile uint32_t*) 0xe000e100 )

// LIMITATION: All interrupts on GPIO pins come in on the same interrupt channel
// Need to make sure that the interrupt channel comes from the correct pin when handling 
// the interrupt


// The GPIO Even IRQ Channel is defined as channel 1
#define ISER0_GPIO_EVEN (1 << 1)

// The GPIO Odd IRQ Channel is defined as channel 11
#define ISER0_GPIO_ODD (1 << 11)


void setupNVIC();

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler();
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler();

void blanking_interrupt();

#endif // _INTERRUPT_HANDLERS_H_