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


typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;


void wait_for_FPGA_ready();
void reset_FPGA();
void clear_object_memory();

void write_sprite_sheet(uint16_t* sprite_sheet, int size);

void write_tile_sheet(uint16_t* tile_sheet, int size);

void write_palette(Color* first_color, int size);

void write_object(Object* obj);

Color* create_palette();

void send_initial_data();

Object obj1;
Object obj2;
Object obj3;
Object obj4;
Object obj5;
Object obj6;
Object obj7;
Object obj8;
Object obj9;
Object obj10;
Object obj11;
Object obj12;
Object obj13;
Object obj14;
Object obj15;
Object obj16;
Object obj17;
Object obj18;
Object obj19;
Object obj20;
Object obj21;


#endif // _FPGA_H_