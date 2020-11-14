#include "spi.h"
#include <stdint.h>


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
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    USART_InitSync_TypeDef spi = USART_INITSYNC_DEFAULT;
    spi.msbf = true;
    spi.master = true;
    spi.autoCsEnable = false;
    spi.baudrate = 9600;
    
    USART_InitSync(USART0, &spi);

    USART0->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC1;

    GPIO_PinModeSet(gpioPortE, 7, gpioModePushPull, 0);     // MOSI
    GPIO_PinModeSet(gpioPortE, 6, gpioModeInput, 0);        // MISO
    GPIO_PinModeSet(gpioPortE, 5, gpioModePushPull, 0);     // Clock

    /// Set CA pins on port E as input
    GPIO_PinModeSet(gpioPortE, 4, gpioModeInput, 1);        //CA_1
    GPIO_PinModeSet(gpioPortE, 3, gpioModeInput, 1);        //CA_2
    GPIO_PinModeSet(gpioPortE, 2, gpioModeInput, 1);        //CA_3
    GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 1);        //CA_4
    GPIO_PinModeSet(gpioPortE, 0, gpioModeInput, 1);        //CA_5

    /// Set CA pins on port B as input
    GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 1);       //CA_6
    GPIO_PinModeSet(gpioPortB, 9 , gpioModeInput, 1);       //CA_7
    GPIO_PinModeSet(gpioPortB, 12, gpioModeInput, 1);       //CA_8
    GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 1);       //CA_9

    /// Set CS pins on port B as output
    GPIO_PinModeSet(gpioPortB, 2, gpioModePushPull, 0);     //CS_1
    GPIO_PinModeSet(gpioPortB, 1, gpioModePushPull, 0);     //CS_2
    GPIO_PinModeSet(gpioPortB, 0, gpioModePushPull, 0);     //CS_3

    /// Set CS pins on port A as output
    GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0);    //CS_4
    GPIO_PinModeSet(gpioPortA, 12, gpioModePushPull, 0);    //CS_5
    GPIO_PinModeSet(gpioPortA, 11, gpioModePushPull, 0);    //CS_6
    GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 0);    //CS_7
    GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull,  0);    //CS_8
    GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull,  0);    //CS_9
}


/***********************************************************
 * @brief Tears down the SPI Driver
 * @returns void
 ***********************************************************/
void teardown_SPI()
{
    //TODO
}


/************************************************************
 * @brief Helper function to receive controller input
 * @returns void
 ***********************************************************/
void receive_ctrl_SPI(int id, uint8_t* buffer_buttons, int8_t* buffer_joystick_x, int8_t* buffer_joystick_y)
{

    // SPI commands for different data, they are delayed by one command
    // 0: Buttons
    // 1: joystick_x
    // 2: joystick_y

    // Throw away the first data we recive since we have
    // not told the controller to fetch anything.
    select_controller(id);
    USART_SpiTransfer(USART0, 0);
    select_controller(-1);

    select_controller(id);
    *buffer_buttons = USART_SpiTransfer(USART0, 1);
    //RETARGET_WriteChar(buttons);
    select_controller(-1);

    select_controller(id);
    *buffer_joystick_x = USART_SpiTransfer(USART0, 2);
    //RETARGET_WriteChar(joystick_x);
    select_controller(-1);

    select_controller(id);
    *buffer_joystick_y = USART_SpiTransfer(USART0, 0);
    //RETARGET_WriteChar(joystick_y);
    select_controller(-1);
}


/************************************************************
 * @brief Helper function to send controller output
 * @returns void
 ***********************************************************/
void send_ctrl_SPI(uint8_t* buffer)
{
    uint8_t inBuf = 0;
    inBuf = USART_SpiTransfer(USART0, *buffer);
}