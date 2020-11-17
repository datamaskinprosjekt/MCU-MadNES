#include "main.h"

// Counts 1ms ticks
volatile uint32_t ticks;

void setup()
{
    CHIP_Init();
    
    // SWO_Setup();

    // Configure core clock
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_4);

    // Setup SysTick Timer for 1 ms interrupts
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10000)) {
        while (1);
    }

    setup_EBI();
    
    setup_NVIC();
    
    setup_SPI();

    initialize_controllers();
}



int main()
{
    setup();
    
    wait_for_FPGA_ready();

    send_initial_data();

    game_loop();

    return 0;
}