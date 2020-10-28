#ifndef _FPGA_TEST_H
#define _FPGA_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "fpga.h"
#include "ebi.h"
#include "em_chip.h"

#define NUM_SPRITES 57
#define SIZE_SHEET (NUM_SPRITES * 256)
#define NUM_COLORS 49
#define SIZE_PALETTE (NUM_COLORS * 3)
#define SIZE_OBJS 1

char* read_file(char* fileName);

void fpga_test();

#endif
