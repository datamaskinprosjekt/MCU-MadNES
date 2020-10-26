#include "fpga_test.h"

void fpga_test() {
    spriteType st1 = {name: ASTEROID, globalSpriteIdx: 0};
    Object obj1 = {id: 1, type: &st1, localSpriteIdx: 1, xPos: 320, yPos: 240, xFlip: false, yFlip: false, enabled: true, priority: 1};
    uint32_t data = write_object_test(&obj1);
}