#include <stdio.h>
#include "main.h"
#include "debug_output.h"
#include "game_logic.h"


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

    initialize_controllers();

    //clear_sprite_sheet(NUM_SPRITES * 256);
    //clear_object_memory();

    send_initial_data();

    game_loop();

    return 0;
}

int main_alt()
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

    wait_for_FPGA_ready();

    clear_sprite_sheet();
    clear_object_memory();

    send_initial_data_alt();

    for (int i = 0; i < 64; i++) {
        SpriteType spriteType = (SpriteType) {ASTEROID, i % 7, 1};

        Object obj = (Object) {.id = 63-i, .type = &spriteType, .localSpriteIdx = 0, .xPos = (i * 16) % 640, .yPos = 16 + ((i / 20) * 16) % 480, .xFlip = 0, .yFlip = 0, .priority = 0, .enabled = 1};
        write_object(&obj);
    }

    return 0;
}