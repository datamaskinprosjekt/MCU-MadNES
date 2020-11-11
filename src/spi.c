#include "spi.h"
/**************************************************************
 * Main file for SPI Functionality
 * ------------------------------------------------------------
 * Here are all the functions used for utilizing the SPI Driver
 * on the EFM32GG.
 ***************************************************************/

/**************************************************************
 * Initializes the SPI Driver with the predefined configuration
 * SPIDRV_MASTER_USART0.
 * 
 * The pins used for the SPI bus are located on port E. 
 *
 * SPI_CLK  - E5
 * SPI_MISO - E6
 * SPI_MOSI - E7
 * @returns void
 **************************************************************/
void setup_SPI()
{
    SPIDRV_Init_t init_data = SPIDRV_MASTER_USART0;
    handle = &handle_data;

    SPIDRV_Init(handle, &init_data);
}

/***********************************************************
 * @brief Tears down the SPI Driver
 * @returns void
 ***********************************************************/
void teardown_SPI()
{
    handle = &handle_data;
    SPIDRV_DeInit(handle);
}

/************************************************************
 * @brief Helper function to receive controller input
 * @returns void
 ***********************************************************/
void receive_ctrl_SPI(uint8_t* buffer)
{
    handle = &handle_data;
    SPIDRV_SReceiveB(handle, buffer, count_ctrl, timeout_ms_ctrl);
}