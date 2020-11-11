#include "spi.h"

void setup_SPI()
{
    SPIDRV_Init_t init_data = SPIDRV_MASTER_USART0;

    SPIDRV_Init(handle, &init_data);
}

void teardown_SPI()
{
    SPIDRV_DeInit(handle);
}