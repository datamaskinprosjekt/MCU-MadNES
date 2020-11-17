#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#include "controllers.h"


void setup_SPI();
void teardown_SPI();

void receive_ctrl_SPI(int id, uint8_t* buttons, uint8_t* joystick_x, uint8_t* joystick_y);
void send_ctrl_sound(int id, int sound);

#endif // _SPI_H_