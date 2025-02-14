#ifndef _FPGA_TEST_H_
#define _FPGA_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "em_chip.h"

#include "ebi.h"
#include "fpga.h"
#include "interrupt_handlers.h"
#include "sys_time.h"
#include "meta_data.h"

#define M_PI 3.14159265358979323846
#define TICK_DIV 4

#define FPGA_RESET_MASK 0b1 << 7
#define FPGA_RESET_VALUE 1 << 7


void fpga_test();

#endif // _FPGA_TEST_H_
