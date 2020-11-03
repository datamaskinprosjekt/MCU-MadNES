#ifndef _FPGA_TEST_H
#define _FPGA_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "fpga.h"
#include "ebi.h"
#include "em_chip.h"

#define NUM_SPRITES 57
#define NUM_COLORS 49
#define NUM_OBJS 1

char* read_data(char* fileName);
char** parse_data(char* data, int size);
Color* create_palette(char* paletteFile, int num_colors);
char* create_sprite_sheet(char* spriteFile, int num_sprites);

void fpga_test();

#endif
