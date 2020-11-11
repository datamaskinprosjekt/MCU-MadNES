#ifndef _CONTROLLERS_H_
#define _CONTROLLERS_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_gpio.h"


/// Mask for pins 9-12 on PORT B
#define CTRL_PORT_B_MASK 0b1111000000000

/// Mask for pins 0-4 on PORT E
#define CTRL_PORT_E_MASK 0b0000000011111


typedef struct {
    int id;
    bool enabled;
    uint8_t joyDir;
    bool joyBtn;
    bool btn1;
    bool btn2;
} Controller;


/**************************************************************
 * Enables the controller gpio pins for input.
 * 
 * CA1 = PE4
 * CA2 = PE3
 * CA3 = PE2
 * CA4 = PE1
 * CA5 = PE0
 * CA6 = PB10
 * CA7 = PB9
 * CA8 = PB12
 * CA9 = PB11
 **************************************************************/
void setup_controller_gpio();


/**************************************************************
 * Chip Select Pins 
 * ------------------------------------------------------------
 * CS1 = PB2
 * CS2 = PB1
 * CS3 = PB0
 * 
 * CS4 = PA13
 * CS5 = PA12
 * CS6 = PA11
 * CS7 = PA10
 * CS8 = PA9
 * CS9 = PA8
 **************************************************************/

/*************************************************************
 * Gets the active controllers.
 *************************************************************/
int get_controllers();


/*************************************************************
 * Initializes Controller structs for all active controllers.
 *************************************************************/
void initialize_controllers();


#endif