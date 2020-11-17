#include "spi.h"


/**************************************************************
 * SPI Functionality
 * ------------------------------------------------------------
 * Here are all the functions for utilizing the SPI driver
 * on the EFM32GG.
 **************************************************************/


/**************************************************************
 * @brief Sets up the SPI for the controllers.
 * 
 * Configures the SPI driver with the predefined configuration
 * SPIDRV_MASTER_USART0.
 * Initializes USART0 in synchronized mode.
 * Disables automatic chipselect and sets baudrate.
 * Sets correct GPIO pin modes for CS and CA pins.
 **************************************************************/
void setup_SPI()
{
    // Enable HFPER, USART0 and GPIO clock
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Configure the SPI driver
    USART_InitSync_TypeDef spi = USART_INITSYNC_DEFAULT;
    spi.autoCsEnable = false;
    spi.msbf = true;
    spi.baudrate = 9600;
    
    // Initialize USART0 in synchronous mode
    USART_InitSync(USART0, &spi);

    USART0->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC1;

    // Set GPIO pin modes

    GPIO_PinModeSet(gpioPortE, 7, gpioModePushPull, 0);     // MOSI
    GPIO_PinModeSet(gpioPortE, 6, gpioModeInput, 0);        // MISO
    GPIO_PinModeSet(gpioPortE, 5, gpioModePushPull, 0);     // Clock

    // Set CA pins on port E as input
    GPIO_PinModeSet(gpioPortE, 4, gpioModeInputPull, 1);    // CA_1
    GPIO_PinModeSet(gpioPortE, 3, gpioModeInputPull, 1);    // CA_2
    GPIO_PinModeSet(gpioPortE, 2, gpioModeInputPull, 1);    // CA_3
    GPIO_PinModeSet(gpioPortE, 1, gpioModeInputPull, 1);    // CA_4
    GPIO_PinModeSet(gpioPortE, 0, gpioModeInputPull, 1);    // CA_5

    // Set CA pins on port B as input
    GPIO_PinModeSet(gpioPortB, 10, gpioModeInputPull, 1);   // CA_6
    GPIO_PinModeSet(gpioPortB, 9 , gpioModeInputPull, 1);   // CA_7
    GPIO_PinModeSet(gpioPortB, 12, gpioModeInputPull, 1);   // CA_8

    // The on-board controller is not connected
    // GPIO_PinModeSet(gpioPortB, 11, gpioModeInputPull, 1); // CA_9

    // Set CS pins on port B as output
    GPIO_PinModeSet(gpioPortB, 2, gpioModePushPull, 0);     // CS_1
    GPIO_PinModeSet(gpioPortB, 1, gpioModePushPull, 0);     // CS_2
    GPIO_PinModeSet(gpioPortB, 0, gpioModePushPull, 0);     // CS_3

    // Set CS pins on port A as output
    GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0);    // CS_4
    GPIO_PinModeSet(gpioPortA, 12, gpioModePushPull, 0);    // CS_5
    GPIO_PinModeSet(gpioPortA, 11, gpioModePushPull, 0);    // CS_6
    GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 0);    // CS_7
    GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull,  0);    // CS_8

    // The on-board controller is not connected
    // GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull,  0); // CS_9
}


/**************************************************************
 * @brief Tears down the SPI for the controllers.
 * 
 * Sets GPIO pin modes to Disabled for CS and CA pins.
 * Resets USART0.
 * Disables the HFPER and USART0 clocks.
 **************************************************************/
void teardown_SPI()
{
    // Set GPIO pin modes

    GPIO_PinModeSet(gpioPortE, 7, gpioModeDisabled, 0);     // MOSI
    GPIO_PinModeSet(gpioPortE, 6, gpioModeDisabled, 0);        // MISO
    GPIO_PinModeSet(gpioPortE, 5, gpioModeDisabled, 0);     // Clock

    // Set CA pins on port E as input
    GPIO_PinModeSet(gpioPortE, 4, gpioModeDisabled, 1);    // CA_1
    GPIO_PinModeSet(gpioPortE, 3, gpioModeDisabled, 1);    // CA_2
    GPIO_PinModeSet(gpioPortE, 2, gpioModeDisabled, 1);    // CA_3
    GPIO_PinModeSet(gpioPortE, 1, gpioModeDisabled, 1);    // CA_4
    GPIO_PinModeSet(gpioPortE, 0, gpioModeDisabled, 1);    // CA_5

    // Set CA pins on port B as input
    GPIO_PinModeSet(gpioPortB, 10, gpioModeDisabled, 1);   // CA_6
    GPIO_PinModeSet(gpioPortB, 9 , gpioModeDisabled, 1);   // CA_7
    GPIO_PinModeSet(gpioPortB, 12, gpioModeDisabled, 1);   // CA_8

    // The on-board controller is not connected
    // GPIO_PinModeSet(gpioPortB, 11, gpioModeDisabled, 1); // CA_9

    // Set CS pins on port B as output
    GPIO_PinModeSet(gpioPortB, 2, gpioModeDisabled, 0);     // CS_1
    GPIO_PinModeSet(gpioPortB, 1, gpioModeDisabled, 0);     // CS_2
    GPIO_PinModeSet(gpioPortB, 0, gpioModeDisabled, 0);     // CS_3

    // Set CS pins on port A as output
    GPIO_PinModeSet(gpioPortA, 13, gpioModeDisabled, 0);    // CS_4
    GPIO_PinModeSet(gpioPortA, 12, gpioModeDisabled, 0);    // CS_5
    GPIO_PinModeSet(gpioPortA, 11, gpioModeDisabled, 0);    // CS_6
    GPIO_PinModeSet(gpioPortA, 10, gpioModeDisabled, 0);    // CS_7
    GPIO_PinModeSet(gpioPortA, 9, gpioModeDisabled,  0);    // CS_8

    // The on-board controller is not connected
    // GPIO_PinModeSet(gpioPortA, 8, gpioModeDisabled,  0); // CS_9

    // Reset USART0
    USART_Reset(USART0);

    // Disable HFPER and USART0 clock
    CMU_ClockEnable(cmuClock_HFPER, false);
    CMU_ClockEnable(cmuClock_USART0, false);
}

/**************************************************************
 * @brief Helper function to receive controller input
 * Receives data from a controller and stores it in the
 * specified buffers.
 * 
 * @param id The id of the controller to receive from.
 * @param buttons A pointer to a buffer storing button data.
 * @param joystick_x A pointer to a buffer storing joystick x position data.
 * @param joystick_x A pointer to a buffer storing joystick y position data.
 **************************************************************/
void receive_ctrl_SPI(int id, uint8_t* buttons, uint8_t* joystick_x, uint8_t* joystick_y)
{

    // SPI commands for different data, they are delayed by one command
    // The first data received is therefore garbage and is thrown away

    // 0: Buttons
    // 1: joystick_x
    // 2: joystick_y

    select_controller(id);
    USART_SpiTransfer(USART0, 0);
    select_controller(-1);

    select_controller(id);
    *buttons = USART_SpiTransfer(USART0, 1);
    select_controller(-1);

    select_controller(id);
    *joystick_x = USART_SpiTransfer(USART0, 2);
    select_controller(-1);

    select_controller(id);
    *joystick_y = USART_SpiTransfer(USART0, 0);
    select_controller(-1);
    
    // Delay to stabalize select signals
    delay(25/1000);
}