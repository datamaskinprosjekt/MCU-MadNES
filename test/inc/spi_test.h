#ifndef _SPI_TEST_H_
#define _SPI_TEST_H_

#include <stdlib.h>
#include <stdint.h>

#include "em_chip.h"

#include "fpga.h"
#include "controllers.h"
#include "object.h"
#include "meta_data.h"

void spi_test();

bool spi_test_sem;

#endif // _SPI_TEST_H_