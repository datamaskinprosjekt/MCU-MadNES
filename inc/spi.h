#ifndef _SPI_H_
#define _SPI_H_

#include "spidrv.h"


SPIDRV_HandleData_t handle_data;
SPIDRV_Handle_t handle = &handle_data;

void setup_SPI();

#endif // _SPI_H_