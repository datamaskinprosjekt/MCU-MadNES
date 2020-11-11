#ifndef _FPGA_TEST_H_
#define _FPGA_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "em_chip.h"
#include "ebi.h"
#include "fpga.h"
#include "interrupt_handlers.h"
#include "time.h"
#include "meta_data.h"


#define NUM_SPRITES 57
#define NUM_COLORS 49

#define FPGA_RESET_MASK 0b1 << 7
#define FPGA_RESET_VALUE 1 << 7


void fpga_test();

#endif // _FPGA_TEST_H_
