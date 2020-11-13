#include "main.h"
#include "debug_output.h"
#include <stdio.h>


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
    SWO_Setup();

    // Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 1 msec interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
        while (1);
    }

    //setup_EBI();

    //setup_NVIC();

    //GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);
    //while(GPIO_PinInGet(gpioPortD, 1) == 0);

    //delay(1000);

    setup_SPI();

    uint8_t outBuf = 0b01010101;
    uint8_t trowaway = 0;
    uint8_t buttons = 0;
    uint8_t joystick_x = 0;
    uint8_t joystick_y = 0;

    while(1) {

        outBuf = 0b01010111;
        trowaway = 0;

        // 00000001: A
        // 00000010: B
        // 00000100: Joystick Button 
        buttons = 0;
        joystick_x = 0; // 8-Bit resolution
        joystick_y = 0; // 8-Bit resolution
        // SPI commands for different data, they are delayed by one command
        // 0: Buttons
        // 1: joystick_x
        // 2: joystick_y

        // We throw away the first data we recive since we have not told the controller to fetch anything.
        // Total 4 SPI transactions need to be send for each controller.
        select_controller(0);
        trowaway = USART_SpiTransfer(USART0, 0);
        select_controller(-1);

        select_controller(0);
        buttons = USART_SpiTransfer(USART0, 1);
        RETARGET_WriteChar(buttons);
        select_controller(-1);

        select_controller(0);
        joystick_x = USART_SpiTransfer(USART0, 2);
        RETARGET_WriteChar(joystick_x);
        select_controller(-1);

        select_controller(0);
        joystick_y = USART_SpiTransfer(USART0, 0);
        RETARGET_WriteChar(joystick_y);
        select_controller(-1);

        delay(16.7);
    }

    //send_initial_data();

    //fpga_test();
    /* GAME LOOP */

    /* END GAME */

    /* RESET */

    return 0;
}

int main_alt()
{
    fpga_test();
    //while(1);
    return 0;
}