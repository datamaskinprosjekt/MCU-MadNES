#ifndef _SPI_H_
#define _SPI_H_


#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "controllers.h"


void setup_SPI();
void teardown_SPI();


void usart_transfer_spi(uint8_t* inBuf, uint8_t* outBuf);
void usart_receive_single_spi(uint8_t* inBuf);


/*************************************************************
 * @brief Helper function to receive controller input
 * Receives data from a controller and stores it in the
 * specified buffer.
 * 
 * @param buffer Pointer to data storage.
 *************************************************************/
void receive_ctrl_SPI(int id, uint8_t* buffer_buttons, int8_t* buffer_joystick_x, int8_t* buffer_joystick_y);
void send_ctrl_SPI(uint8_t* buffer);

#endif // _SPI_H_