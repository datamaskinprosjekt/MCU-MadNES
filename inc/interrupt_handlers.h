#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

#include <stdint.h>
#include <stdbool.h>

#include "em_gpio.h"
#include "fpga.h"
#include "ebi.h"


// NOTE: SDK Method to interface with ISER?
#define ISER0 ((volatile uint32_t*) 0xe000e100 )

// LIMITATION: All interrupts on GPIO pins come in on the same interrupt channel
// Need to make sure that the interrupt channel comes from the correct pin when handling 
// the interrupt



// The GPIO Even IRQ Channel is defined as channel 1
#define ISER0_GPIO_EVEN (1 << 1)

// The GPIO Odd IRQ Channel is defined as channel 11
#define ISER0_GPIO_ODD (1 << 11)



/**
 * setupNVIC
 * ============================================================
 * Sets up the nested vector interrupt controller to enable 
 * handling the interrupts as they come in
 */ 
void setupNVIC();


/**
 * Interrupt Handlers for Blanking Phase
 * ============================================================
 * Handles the interrupt signal that comes in as the FPGA enters
 * the Vertical Blank phase.
 * This time is spent writing to the FPGA memory to update the state
 * of objects and the control registers.
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler();
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler();

void blanking_interrupt();

#endif // INTERRUPT_HANDLERS_H