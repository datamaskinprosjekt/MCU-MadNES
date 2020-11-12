#include "main.h"


// Counts 1ms ticks
volatile uint32_t ticks;

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

    // Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 1 msec interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
        while (1);
    }

    //setup_EBI();

    //setup_NVIC();

    GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);
    while(GPIO_PinInGet(gpioPortD, 1) == 0);

    //delay(1000);

    setup_SPI();
    setup_controller_gpio();

    //send_initial_data();

    //int transferCount = 0;

    send_to_controller(8);

    while(1)
        //poll_single_controller(8);
        send_to_controller(8);
        //transferCount++;

    while(1) {
        //poll_single_controller(8);
        send_to_controller(8);
        //GPIO_PinOutSet(gpioPortE, 7);
    }


    //fpga_test();
    /* GAME LOOP */

    /* END GAME */

    /* RESET */

    return 0;
}

int main()
{
    fpga_test();
    //while(1);
    return 0;
}