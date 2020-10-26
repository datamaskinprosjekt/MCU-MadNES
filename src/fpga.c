#include "fpga.h"

#define SIZE_OBJECTS

void write_initial_data() {
    //Needs timing
}

void write_sprite_sheet() {
    //TODO
    //Write sprite data using EBI bank 1 and CS 1
}

void write_tile_sheet() {
    //TODO
    //Write tile data using EBI bank 2 and CS 2
}

void write_palettes() {
    //TODO
    //Write palette data using EBI bank 3 and CS 3
}

void write_object(Object* obj) {

    //Write object data using EBI bank 3 and CS 3
    uint32_t data = 0; //[1:enabled][1:priority][1:yFlip][1:xFlip][20:xyPos][8:spriteId]
    uint16_t addr = fpgaAddr3 + sizeof(uint32_t) * obj->id;

    //SpriteId
    uint8_t spriteId = obj->type->globalSpriteIdx + obj->localSpriteIdx;

    data |= (uint32_t) spriteId;

    //xyPos
    data |= (uint32_t) obj->xPos << 8;
    data &= ~(1 << 18);
    data &= ~(1 << 19);
    data &= ~(1 << 20);
    data &= ~(1 << 21);
    data &= ~(1 << 22);

    data |= (uint32_t) obj->yPos << 18;
    data &= ~(1 << 27);
    data &= ~(1 << 28);
    data &= ~(1 << 29);
    data &= ~(1 << 30);
    data &= ~(1 << 31);

    //xFlip
    if (obj->xFlip) {
        data |= 1 << 28;
    }

    //yFlip
    if (obj->yFlip) {
        data |= 1 << 29;
    }

    //Enabled
    if (obj->enabled) {
        data |= 1 << 31;
    }

    //Priority
    if (obj->priority) {
        data |= 1 << 30;
    }
    
    //Fix this!
    addr = data;
}

void update_palette(uint8_t paletteId) {
    //Write paletteId to some (currently unknown) address
    //TODO
}

void update_background() {
    //TODO
}