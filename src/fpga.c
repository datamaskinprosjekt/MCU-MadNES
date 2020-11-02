#include "fpga.h"

void write_sprite_sheet(char* spriteSheet) {
    char id = 0;
    char* next = spriteSheet + id;

    set_bank(SPRITE);

    while (next != '\0') {
        *(fpgaAddr0 + id) = *next;
        id++;
        next = spriteSheet + id;
    }

    clear_bank(SPRITE);
}

void write_tile_sheet() {
    //TODO
}

void write_palette(Color* firstColor, int size) {
    uint16_t* addr = fpgaAddr0;
    
    for (int id = 0; id < size; id++) {
        addr += sizeof(uint16_t) * id;

        Color* color = (firstColor + id);

        // Possible undefined behavior, must allocate an additional byte
        // in color palette to avoid accessing unallocated memory
        uint16_t RG_Pair = (color->r << 8) + color->g;
        uint16_t B_Single = color->b;

        set_bank(PALETTE);
        
        *addr = RG_Pair;
        *(addr + 1) = B_Single;

        bank_clear();
    }
}

void write_object(Object* obj) {

    uint32_t data = 0; //[1:enabled][1:priority][1:yFlip][1:xFlip][20:xyPos][8:spriteId]
    uint16_t* addr = fpgaAddr0 + sizeof(uint16_t) * obj->id;

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
    
    uint16_t* data1 = (uint16_t*) &data;
    uint16_t* data2 = (uint16_t*) &data + 1;

    set_bank(OAM);

    *addr = *data1;
    *(addr + 1) = *data2;

    clear_bank();
}