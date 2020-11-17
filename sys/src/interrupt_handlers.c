#include "interrupt_handlers.h"

/**************************************************************
 * Interrupt Handlers
 * ------------------------------------------------------------
 * This file contains all the functions needed to handle
 * the interrupts needed for the MadNES Console CPU.
 *************************************************************/


/**************************************************************
 * Registers a function pointer to be called in the blanking interrupt
 * 
 * @see blanking_interrupt
 **************************************************************/
void register_interrupt_callback(void (*callback)())
{
    user_interrupt_callback = callback;
}

/**************************************************************
 * @brief Sets up GPIO interrupt pins
 * 
 * Sets up interrupt on the B4 pin. The FPGA will send a high pulse
 * whenever it enters the vertical blank phase.
 **************************************************************/
void setup_NVIC()
{

    // Set the ready-state to false for interrupts
    __SYS_INTERRUPTS_READY = false;

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
        interrupt_port,              // Which port is the pin we want to enable interrupts for on?
        interrupt_pin,               // Which pin to enable interrupts on?
        interrupt_num,               // The interrupt number to trigger
        trigger_on_rising_edge,      // Trigger on rising edge?
        trigger_on_falling_edge,     // Trigger on falling edge?
        true                         // Enable
    );

    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

/**************************************************************
 * Must be called by the game to enable writing dirty objects
 * 
 * @see blanking_interrupt
 *************************************************************/
void __game_ready() { __SYS_INTERRUPTS_READY = true; }


/**************************************************************
 * @brief Blanking Phase interrupt handler
 * 
 * When this interrupt handler is called, the FPGA has entered the vertical blank phase.
 * The interrupt handler will call a user-registered function
 * 
 * @see register_interrupt_callback
 *************************************************************/ 
void blanking_interrupt()
{
    if(user_interrupt_callback != NULL)
        user_interrupt_callback();

    // Check whether the game has initialized all objects
    // before handling any object writing
    if(!__SYS_INTERRUPTS_READY) return;

    Object* dirty_obj;

    for (int i = 0; i < MAX_OBJS; i++) {
       if (dirty_objects[i]) {
           dirty_obj = &objects[i];
           write_object(dirty_obj);

           if(dirty_obj->type == SCORE) {
               int breakhere = 3;
           }
       }
    }

    memset(dirty_objects, 0, sizeof(int) * MAX_OBJS);
}


/**************************************************************
 * Definition of the interrupt handler for the GPIO_EVEN IRQ channel 
 *************************************************************/
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
    uint32_t pending_interrupts = GPIO_IntGet();

    blanking_interrupt();

    GPIO_IntClear(pending_interrupts);
}

/**************************************************************
 * Definition of the interrupt handler for the GPIO_ODD IRQ channel 
 *************************************************************/
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
    uint32_t pending_interrupts = GPIO_IntGet();

    blanking_interrupt();

    GPIO_IntClear(pending_interrupts);
}
