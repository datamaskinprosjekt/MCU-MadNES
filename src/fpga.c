#include "fpga.h"

void write_sprite_sheet(uint16_t* spriteSheet, int size) {
    fpgaAddr0 = (uint16_t *) EBI_BankAddress(EBI_BANK0);
    for (int id = 0; id < size; id++) {
        set_bank(SPRITE);
        *(fpgaAddr0 + id) = *(spriteSheet + id);
        clear_bank();
    }
}

void write_tile_sheet() {
    //TODO
}

void write_palette(Color* palette, int size) {
    fpgaAddr0 = (uint16_t *) EBI_BankAddress(EBI_BANK0);
    uint16_t* addr = fpgaAddr0;
    
    for (int id = 0; id < size; id++) {
        addr += sizeof(uint16_t) * id;

        Color* color = palette + id;

        uint16_t RG_Pair = (color->r << 8) + color->g;
        uint16_t B_Single = color->b;

        set_bank(PALETTE);
        
        *addr = RG_Pair;
        *(addr + 1) = B_Single;

        clear_bank();
    }
}

void write_object(Object* obj) {
    fpgaAddr0 = (uint16_t *) EBI_BankAddress(EBI_BANK0);

    uint32_t data = 0; //[1:enabled][1:priority][1:yFlip][1:xFlip][20:xyPos][8:spriteId]
    uint16_t* addr = (uint16_t*) fpgaAddr0 + 2 * obj->id;

    //SpriteId
    uint8_t spriteId = obj->type->globalSpriteIdx + obj->localSpriteIdx;

    data |= (uint32_t) spriteId;

    //xyPos
 	//xyPos
	data |= (uint32_t) obj->xPos << 8;
	data &= ~(1 << 18);
	data &= ~(1 << 19);
	data &= ~(1 << 20);
	data &= ~(1 << 21);


	data |= (uint32_t) obj->yPos << 18;
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

    //Priority
    if (obj->priority) {
        data |= 1 << 30;
    }

    //Enabled
    if (obj->enabled) {
        data |= 1 << 31;
    }
    
    uint16_t* data1 = (uint16_t*) &data;
    uint16_t* data2 = (uint16_t*) &data + 1;

    set_bank(OAM);

    *addr = *data1;
    *(addr + 1) = *data2;

    clear_bank();

}