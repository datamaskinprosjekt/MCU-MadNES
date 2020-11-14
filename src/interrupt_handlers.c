#include "interrupt_handlers.h"
#include "spi_test.h"

/**
 * setupNVIC()
 * ============================================================
 * Sets up interrupt pin(s) and registers their callbacks.
 * TODO: Desribe which interrupts are being set up
 */
void setup_NVIC()
{
    // Port of the interrupt pin
    int  const interrupt_port = gpioPortB;

    // Pin number
    int  const interrupt_pin  = 4;

    // Interrupt number to trigger
    int const interrupt_num = 4;

    // Trigger interrupt on rising edge?
    bool const trigger_on_rising_edge    = true;

    // Trigger interrupt on falling edge?
    bool const trigger_on_falling_edge   = false;

    GPIO_PinModeSet(interrupt_port, interrupt_pin, gpioModeInputPull, 1);
    
    // Clear interrupts before enable
    GPIO_IntClear(ISER0_GPIO_ODD | ISER0_GPIO_EVEN);

    // Enable interrupts for odd and even GPIO channels
    GPIO_IntEnable(ISER0_GPIO_ODD | ISER0_GPIO_EVEN);

    // When the interrupt pin sees a positive edge, the NVIC Interrupt handler will be called on the 
    // GPIO_IRQ_EVEN and GPIO_IRQ_ODD channels
    GPIO_ExtIntConfig(
        interrupt_port,      // Which port is the pin we want to enable interrupts for on?
        interrupt_pin,       // Which pin to enable interrupts on?
        interrupt_num,       // The interrupt number to trigger
        trigger_on_rising_edge,         // Trigger on rising edge?
        trigger_on_falling_edge,        // Trigger on falling edge?
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
void blanking_interrupt()
{
    Object* dirty_obj;

    for (int i = 0; i < MAX_OBJS; i++) {
       if (dirty_objects[i]) {
           dirty_obj = &objects[i];
           write_object(dirty_obj);
       }
    }

    memset(dirty_objects, 0, sizeof(int) * MAX_OBJS);

    spi_test_sem = true;
}


/**
 * Definition of the EVEN and ODD Interrupt Handler
 * Declared in the EFM32GG EMLIB Source Code *
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
    uint32_t pending_interrupts = GPIO_IntGet();

    blanking_interrupt();

    GPIO_IntClear(pending_interrupts);
}


void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
    uint32_t pending_interrupts = GPIO_IntGet();

    blanking_interrupt();

    GPIO_IntClear(pending_interrupts);

    spi_test_sem = true;
}
