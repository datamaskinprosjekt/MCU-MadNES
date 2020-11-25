#include "spi_test.h"

/**************************************************************
 * Test for SPI functionality.
 *************************************************************/

// Counts 1ms ticks
volatile uint32_t msTicks;

void joystick_handler(int rot, Object* shipObj)
{
    int dir = rot / 4;
    int spriteIdx = rot % 4;
    int xFlip = 0;
    int yFlip = 0;

    switch(dir) { case 1:
            spriteIdx = 8 - rot;
            xFlip = rot == 4? 0 : 1;
            break;
        case 2:
            spriteIdx = rot - 8;
            xFlip = 1;
            yFlip = rot == 8? 0 : 1;
            break;
        case 3:
            spriteIdx = 16 - rot;
            yFlip = 1;
            break;
    }

    shipObj->localSpriteIdx = spriteIdx;
    shipObj->xFlip = xFlip;
    shipObj->yFlip = yFlip;

    dirty_objects[shipObj->id] = 1;
}

void spi_test()
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
    setup_NVIC();
    setup_SPI();

    wait_for_fpga_ready();

    initialize_controllers();

    send_initial_data();


    SpriteType st0 = {.name = SHIP, .globalSpriteIdx = 0, .length = 5};
    Object obj0 = (Object) {.id = 0, .type = &st0, .localSpriteIdx = 0, .xPos = 0, .yPos = 0, .xFlip = false, .yFlip = false, .priority = 0, .enabled = false};
    Object obj1 = (Object) {.id = 1, .type = &st0, .localSpriteIdx = 0, .xPos = 320, .yPos = 240, .xFlip = false, .yFlip = false, .priority = 1, .enabled = true};

    objects[0] = obj0;
    objects[1] = obj1;

    for (int i = 0; i < 64; i++) {
        obj0.id = i;
        write_object(&obj0);
    }

    spi_test_sem = true;

    int counter = 0;

    write_object(&obj1);

    while(1) {

        while(!spi_test_sem);

        spi_test_sem = false;

        memset(&dirty_objects, 0, sizeof(int)*MAX_OBJS);

        poll_single_controller(0);

        int rot = CONTROLLER_INPUTS[0].joyDir;

        if(CONTROLLER_INPUTS[0].joyDir == 2) {
            objects[1].xPos = (objects[1].xPos + 1) % 640;
            objects[1].yPos = (objects[1].yPos + 1) % 480;

            dirty_objects[1] = 1;
        }
    }
}
