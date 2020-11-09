#ifndef _FPGA_TEST_H
#define _FPGA_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "fpga.h"
#include "ebi.h"
#include "interrupt_handlers.h"
#include "em_chip.h"

#define NUM_SPRITES 57
#define NUM_COLORS 49
#define NUM_OBJS 1

#define FPGA_RESET_MASK 0b1 << 7
#define FPGA_RESET_VALUE 1 << 7

char* read_data(char* fileName);
char** parse_data(char* data, int size);
Color* create_palette(int num_colors);
char* create_sprite_sheet(int num_sprites);

void fpga_test();

#endif
