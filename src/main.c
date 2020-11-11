#include "main.h"

// Counts 1ms ticks
volatile uint32_t ticks;

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
    //CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 1 msec interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
        while (1);
    }

    setup_EBI();

    setup_NVIC();

    setup_controller_gpio();
    setup_SPI();

    //GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 3, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 5, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 7, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 10, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 12, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 13, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 14, gpioModePushPull, 1);
    //GPIO_PinModeSet(gpioPortC, 15, gpioModePushPull, 1);

    //GPIO_PinModeSet(gpioPortA, 14, gpioModePushPull, 1);
    //GPIO_PortOutSet(gpioPortA, 0b0100000000000000);

    //GPIO_PortOutSet(gpioPortE, 0xFFFF);
    //GPIO_PortOutSet(gpioPortC, 0xFFFF);

    send_initial_data();

    while(1)
        poll_single_controller(8);
        //send_to_controller(8);

    /* GAME LOOP */

    /* END GAME */

    /* RESET */

    return 0;
}

int main_alt()
{
    //fpga_test();
    while(1);
    return 0;
}