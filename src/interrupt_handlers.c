#include "interrupt_handlers.h"

#include <stdbool.h>
#include "fpga.h"
#include "ebi.h"
#include "em_gpio.h"

// TEMPORARY
extern Object obj1;

/**
 * setupNVIC()
 * ============================================================
 * Sets up interrupt pin(s) and registers their callbacks.
 */
void setupNVIC()
{
    // Port of the interrupt pin
    int  const interruptPort = gpioPortB;

    // Pin number
    int  const interruptPin  = 4;

    // Interrupt number to trigger
    int const interruptNum = 4;

    // Trigger interrupt on rising edge?
    bool const risingEdge    = true;

    // Trigger interrupt on falling edge?
    bool const fallingEdge   = false;

    GPIO_PinModeSet(interruptPort, interruptPin, gpioModeInputPull, 1);
    
    // Clear interrupts before enable
    GPIO_IntClear(ISER0_GPIO_ODD | ISER0_GPIO_EVEN);

    // Enable interrupts for odd and even GPIO channels
    GPIO_IntEnable(ISER0_GPIO_ODD | ISER0_GPIO_EVEN);

    // When the interrupt pin sees a positive edge, the NVIC Interrupt handler will be called on the 
    // GPIO_IRQ_EVEN and GPIO_IRQ_ODD channels
    GPIO_ExtIntConfig(
        interruptPort,      // Which port is the pin we want to enable interrupts for on?
        interruptPin,       // Which pin to enable interrupts on?
        interruptNum,       // The interrupt number to trigger
        risingEdge,         // Trigger on rising edge?
        fallingEdge,        // Trigger on falling edge?
        true                // Enable
    );

    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

}


/**
 * Blanking Phase interrupt handler
 * ============================================================
 * When this handler is called, the FPGA has entered the vertical blank phase.
 * Use this function to write the state / data changes to the FPGA
 */ 

void Blanking_Interrupt()
{
    write_object(&obj1);
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
    uint32_t pendingInterrupts = GPIO_IntGet();

    Blanking_Interrupt();

    GPIO_IntClear(pendingInterrupts);
}
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
    uint32_t pendingInterrupts = GPIO_IntGet();

    Blanking_Interrupt();

    GPIO_IntClear(pendingInterrupts);
}
