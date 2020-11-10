#include "fpga.h"

/**************************************************************
 * Writes a sprite sheet to the SPRITE Memory Bank on the FPGA.
 * @param sprite_sheet A pointer to the first element of the sprite sheet to be written
 *                     to the FPGA.
 * @param size         The number of elements in the Sprite Sheet.
 *************************************************************/
void write_sprite_sheet(uint16_t* sprite_sheet, int size)
{
    for (int i = 0; i < size; i++) {
        set_bank(SPRITE);
        *(FPGA_ADDR + i) = *(sprite_sheet + i);
        clear_bank();
    }
}


/**************************************************************
 * Writes a tile sheet to the TILE Memory Bank on the FPGA.
 * @param tile_sheet A pointer to the first element of the tile sheet to be written
 *                     to the FPGA.
 * @param size         The number of elements in the Sprite Sheet.
 *************************************************************/
void write_tile_sheet(uint16_t* tile_sheet, int size)
{
    for (int i = 0; i < size; i++) {
        set_bank(TILE);
        *(FPGA_ADDR + i) = *(tile_sheet + i);
        clear_bank();
    }
}

/*************************************************************
 * Writes a color palette to the FPGA.
 * 
 * @param palette A pointer to the first `Color` element of the palette.
 * @param size    The number of `Color` elements to be written to the FPGA.
 *************************************************************/
void write_palette(Color* palette, int size)
{
    uint16_t* addr = FPGA_ADDR;
    
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


/**************************************************************
 * Writes an Object to the OAM Bank of the FPGA.
 * 
 * @param obj The Object to be written.
 **************************************************************/ 
void write_object(Object* obj)
{
    uint32_t data = 0; // [1:enabled][1:priority][1:yFlip][1:xFlip][20:xyPos][8:spriteId]
    uint16_t* addr = FPGA_ADDR + 2 * obj->id;

    // SpriteId
    uint8_t spriteId = obj->type->globalSpriteIdx + obj->localSpriteIdx;

    data |= (uint32_t) spriteId;

    // xyPos
	data |= (uint32_t) obj->xPos << 8;
    data &= ~(0b1111 << 18);

	data |= (uint32_t) obj->yPos << 18;
    data &= ~(0b1111 << 28);

    // xFlip
    if (obj->xFlip) {
        data |= 1 << 28;
    }

    // yFlip
    if (obj->yFlip) {
        data |= 1 << 29;
    }

    // Priority
    if (obj->priority) {
        data |= 1 << 30;
    }

    // Enabled
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


/**************************************************************
 * Creates a palette of Colors from the palette data.
 * 
 * @param num_colors The number of colors in the palette.
 **************************************************************/ 
Color* create_palette()
{
	Color* palette;
    int i, j;
		
	palette = (Color*) malloc(NUM_COLORS * sizeof(void*));
	i = 0;
	
	for (j = 0; j < (NUM_COLORS * 3); j += 3) {
		Color color = {.r = (uint8_t) palette_data[j], .g = (uint8_t) palette_data[j+1], .b = (uint8_t) palette_data[j+2]};
		palette[i] = color;
		i++;
	}
	
	return palette;
}


/**************************************************************
 * Sends initial data consisting of tile sheet, sprite sheet
 * and palette to the FPGA.
 **************************************************************/
void send_initial_data()
{
    Color* palette;
    palette = create_palette(NUM_COLORS);

    // TODO: add tile sheet
    write_sprite_sheet((uint16_t*) sprite_data, NUM_SPRITES * 256);
    write_palette(palette, NUM_COLORS);
}