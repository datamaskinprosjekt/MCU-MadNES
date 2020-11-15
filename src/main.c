#include <stdio.h>
#include "main.h"
#include "debug_output.h"
//#include "game_logic.h"


// Counts 1ms ticks
volatile uint32_t ticks;

int main()
{
    CHIP_Init();
    SWO_Setup();

    // Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 0.1 msec interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10000)) {
        while (1);
    }

    setup_EBI();

    objects_initialized = false;

    setup_NVIC();
    setup_SPI();

    wait_for_FPGA_ready();

    //initialize_controllers();

    clear_object_memory();

    send_initial_data();
    int x_offset = 0;
    for (int i = 0; i < 7; i++)
    {
         Object obj1;
        obj1.id      = i;
        obj1.xPos    = x_offset; 
        obj1.yPos    = 0; 
        obj1.enabled = 1;
        write_object(&obj1);
        x_offset += 16;
    }

    //game_loop();

    return 0;
}