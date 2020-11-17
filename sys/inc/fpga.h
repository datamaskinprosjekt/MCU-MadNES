#ifndef _FPGA_H_
#define _FPGA_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_ebi.h"
#include "em_gpio.h"
#include "ebi.h"
#include "object.h"
#include "data.h"

#define FPGA_ADDR (uint16_t *) EBI_BankAddress(EBI_BANK0)

// NOTE: Is this the best location for this typedef?
typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;


// FPGA Signalss
void wait_for_FPGA_ready();
void reset_FPGA();

// Object Attribute Memory
void write_object(Object* obj);
void clear_object_memory();

// Sprite Sheet Memory
void write_sprite_sheet(uint16_t* sprite_sheet, int size);
void clear_sprite_memory();

// Tile Attribute Memory
// NOTE: TAM is not supported in this iteration of the console
void write_tile_sheet(uint16_t* tile_sheet, int size);

// Palette Memory
void write_palette(Color* first_color, int size);
void clear_palette_memory();
Color* create_palette();

// Initial data transfer
void send_initial_data();

#endif // _FPGA_H_s