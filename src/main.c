#include "main.h"


// Counts 1ms ticks
volatile uint32_t msTicks;

int main()
{
    // Object dirty_objects

    /* INITIALIZATION */
        // Initialize chip, EBI, NVIC, SPI(?)
        // Enable clock
        // Setup timer interrupts
        // Reset FPGA
        // Send initial data to FPGA: sprite sheet, palette, tile sheet (?)

    CHIP_Init();

    // Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 1 msec interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
        while (1);
    }

    setup_EBI();

    setup_NVIC();
    
    setup_SPI();

    // wait_for_fpga_ready();


    send_initial_data();


    /* GAME LOOP */

    /* END GAME */

    /* RESET */
    // reset_FPGA();

    return 0;
}

int main_alt()
{
    fpga_test();
    //while(1);
    return 0;
}