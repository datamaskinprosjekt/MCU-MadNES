#include "main.h"
#include "debug_output.h"
#include <stdio.h>


// Counts 1ms ticks
volatile uint32_t msTicks;

int main_alt()
{
    // Object dirty_objects

    /* INITIALIZATION */
        // Initialize chip, EBI, NVIC, SPI(?)
        // Enable clock
        // Setup timer interrupts
        // Reset FPGA
        // Send initial data to FPGA: sprite sheet, palette, tile sheet (?)

    CHIP_Init();
    SWO_Setup();

    // Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 0.1 msec interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10000)) {
        while (1);
    }

    //setup_EBI();

    //setup_NVIC();

    setup_SPI();

    //send_initial_data();

    while(1) {
        poll_single_controller(0);
        delay(1000);
    }

    //fpga_test();
    /* GAME LOOP */

    /* END GAME */

    /* RESET */

    return 0;
}

int main()
{
    spi_test();

    return 0;
}