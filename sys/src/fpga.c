#include "fpga.h"


/**************************************************************
 * FPGA Interfacing
 * ------------------------------------------------------------
 * This file contains all the functions needed to handle
 * interfacing with the FPGA and its memory modules.
 * 
 * See ebi.c for specifics on how data-transfers are handled.
 *************************************************************/


/**************************************************************
 * Waits for the FPGA to be ready.
 * 
 * This happens when the DONE signal at pin D1 goes high
 *************************************************************/
void wait_for_FPGA_ready()
{
    // Set the PinMode for reading input
    GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

    // Busy-wait for the signal to go high before proceeding
    while(GPIO_PinInGet(gpioPortD, 1) == 0);

    // Disable the pin, no longer needed
    GPIO_PinModeSet(gpioPortD, 1, gpioModeDisabled, 0);
}


/**************************************************************
 * Resets the FPGA.
 * 
 * The FPGA reset pin is connected to pin A7 on the EFM32GG
 * By sending a high pulse on this pin, the FPGA receives a reset signal
 * on its FPGA_RESET port
 *************************************************************/
void reset_FPGA()
{
    // Set PinMode for output
    GPIO_PinModeSet(gpioPortA, 7, gpioModePushPull, 0);

    // "Pulse" a reset signal
    GPIO_PinOutSet(gpioPortA, 7);
    GPIO_PinOutClear(gpioPortA, 7);

    // Disable the pin
    GPIO_PinModeSet(gpioPortA, 7, gpioModeDisabled, 0);
}


/**************************************************************
 * Writes a sprite sheet to the SPRITE Memory Bank on the FPGA.
 * 
 * @param sprite_sheet A pointer to the first element of the sprite sheet to be written
 *                     to the FPGA.
 * @param size         The number of elements in the Sprite Sheet.
 *************************************************************/
void write_sprite_sheet(uint16_t* sprite_sheet, int size)
{
    for (int i = 0; i < size; i++) {
        set_bank(SPRITE);
        // TODO: Try out with void pointers to address in increments of 1
        *(FPGA_ADDR + i) = *(sprite_sheet + i);
        clear_bank();
    }
}


/*************************************************************
 * Writes a color palette to the FPGA.
 * 
 * Colors written to the FPGA are written like in the following way:
 * - Address 2n ->   RG
 * - Address 2n+1 -> B
 * 
 * @param palette A pointer to the first `Color` element of the palette.
 * @param size    The number of `Color` elements to be written to the FPGA.
 *************************************************************/
void write_palette(Color* palette, int size)
{
    /// Use of void pointer to force addressing increments of 1.
    void* addr = FPGA_ADDR;
    
    // Iterate through the entire palette
    for (int id = 0; id < size; id++) {

        // Calculate the first address to write to for this palette entry
        addr = FPGA_ADDR + sizeof(uint16_t) * id;

        Color* color = palette + id; // palette + id gives the heap-space address
                                     // of the color we are writing

        uint16_t RG_Pair = (color->r << 8) + color->g;
        uint16_t B_Single = color->b;

        set_bank(PALETTE);
        
        *( (uint16_t*) addr) = RG_Pair;
        *( (uint16_t*) addr + 1) = B_Single;

        clear_bank();
    }
}


/**************************************************************
 * @brief Writes an Object to the OAM Bank of the FPGA.
 * 
 * The FPGA expects an OAM entry to be a bit-string in the following format:
 * 
 * | Bits  | Field Name |
 * | ----  | ---------- |
 * | 7:0   | Sprite ID  |
 * | 17:8  | x-position | 
 * | 27:18 | y-position | 
 * | 28:28 | x-flip     |
 * | 29:29 | x-flip     |
 * | 30:30 | priority   |
 * | 31:31 | enabled    |
 * 
 * @param obj The Object to be written.
 **************************************************************/ 
void write_object(Object* obj)
{
    uint32_t data = 0;
    uint16_t* addr = FPGA_ADDR + 2 * obj->id;

    // SpriteId
    uint8_t spriteId = obj->type->globalSpriteIdx + obj->localSpriteIdx;

    data |= (uint32_t) spriteId;

    // X and Y Positions
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

    // TODO: Test this with void pointers for 1-increment addressing
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


/**************************************************************
 * Overwrites the SPRITE bank with all zeroes.
 *************************************************************/
void clear_sprite_memory()
{
    for (int i = 0; i < (NUM_SPRITES *256); i++) {
        set_bank(SPRITE);

        uint16_t zero = 0;

        // Test this with void pointers for 1 increment addressing
        *(FPGA_ADDR + i) = zero;
        clear_bank();
    }
}


/**************************************************************
 * Overwrites the PALETTE bank with all zeroes.
 *************************************************************/
void clear_palette_memory()
{
    for (int i = 0; i < NUM_COLORS; i++) {
        set_bank(PALETTE);

        uint16_t zero = 0;

        // Test this with void pointers for 1 increment addressing
        *((uint16_t*) ((void* )(FPGA_ADDR + i))) = zero;
        *((uint16_t*) ((void* )(FPGA_ADDR + i) + 1)) = zero;
        clear_bank();
    }
}


/**************************************************************
 * Fills the Object Attribute Memory (OAM) 
 * on the FPGA with zero-entries 
 *************************************************************/
void clear_object_memory()
{
    // Initialize a zeroed-out Object
    Object* empty_obj;
    empty_obj = malloc(sizeof(Object));
    memset(empty_obj, 0, sizeof(Object));

    // Write to every OAM location on the FPGA
    for (int i = 0; i < MAX_OBJS; i++) {
        empty_obj->id = i;
        write_object(empty_obj);
    }

    free(empty_obj);
}


/**************************************************************
 * Sends initial data consisting of tile sheet, sprite sheet
 * and palette to the FPGA.
 * 
 * @see create_palette
 * @see write_sprite_sheet
 * @see write_palette
 **************************************************************/
void send_initial_data()
{
    //clear_sprite_memory();
    //clear_palette_memory();
    //clear_object_memory();
    
    Color* palette;
    palette = create_palette(NUM_COLORS);

    write_sprite_sheet((uint16_t*) sprite_data, NUM_SPRITES * 128);
    
    write_palette(palette, NUM_COLORS);
}