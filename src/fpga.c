#include "fpga.h"

/**************************************************************
 * Waits for the FPGA to be ready.
 *************************************************************/
void wait_for_FPGA_ready()
{
    GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

    while(GPIO_PinInGet(gpioPortD, 1) == 0);

    GPIO_PinModeSet(gpioPortD, 1, gpioModeDisabled, 0);
}

/**************************************************************
 * Resets the FPGA.
 *************************************************************/
void reset_FPGA()
{
    GPIO_PinModeSet(gpioPortA, 7, gpioModePushPull, 0);

    // "Pulse" a reset signal
    GPIO_PinOutSet(gpioPortA, 7);
    GPIO_PinOutClear(gpioPortA, 7);

    GPIO_PinModeSet(gpioPortA, 7, gpioModeDisabled, 0);
}

void clear_object_memory()
{
    Object* empty_obj;

    empty_obj = malloc(sizeof(Object));
    memset(empty_obj, 0, sizeof(Object));

    for (int i = 0; i < MAX_OBJS; i++) {
        empty_obj->id = i;
        write_object(empty_obj);
    }

    free(empty_obj);
}


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
    void* addr = FPGA_ADDR;
    
    for (int id = 0; id < size; id++) {
        addr = FPGA_ADDR + sizeof(uint16_t) * id;

        Color* color = palette + id;

        uint16_t RG_Pair = (color->r << 8) + color->g;
        uint16_t B_Single = color->b;

        set_bank(PALETTE);
        
        *( (uint16_t*) addr) = RG_Pair;
        *( (uint16_t*) addr + 1) = B_Single;

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
		Color color = {.r = palette_data[j], .g = palette_data[j+1], .b = palette_data[j+2]};
		palette[i] = color;
		i++;
	}
	
	return palette;
}

void clear_sprite_sheet(int size)
{
    for (int i = 0; i < size; i++) {
        set_bank(SPRITE);

        uint16_t zero = 0;

        *(FPGA_ADDR + i) = zero;
        clear_bank();
    }
}

void clear_palette(int size)
{
    for (int i = 0; i < size; i++) {
        set_bank(PALETTE);

        uint16_t zero = 0;

        *(FPGA_ADDR + i) = zero;
        clear_bank();
    }
}


/**************************************************************
 * Sends initial data consisting of tile sheet, sprite sheet
 * and palette to the FPGA.
 **************************************************************/
void send_initial_data()
{
    Color* palette;
    palette = create_palette(NUM_COLORS);

    write_sprite_sheet((uint16_t*) sprite_data, NUM_SPRITES * 128);
    
    write_palette(palette, NUM_COLORS);
}

// Test

Color* create_palette_alt()
{
	Color* palette;
    int i, j;
		
	palette = (Color*) malloc(NUM_COLORS_ALT * sizeof(void*));
	i = 0;
	
	for (j = 0; j < (NUM_COLORS_ALT * 3); j += 3) {
		Color color = {.r = palette_data_alt[j], .g = palette_data_alt[j+1], .b = palette_data_alt[j+2]};
		palette[i] = color;
		i++;
	}
	
	return palette;
}

void send_initial_data_alt()
{
    Color* palette;
    palette = create_palette_alt(NUM_COLORS_ALT);

    write_sprite_sheet((uint16_t*) sprite_data_alt, NUM_SPRITES_ALT * 128);
    
    write_palette(palette, NUM_COLORS_ALT);
}