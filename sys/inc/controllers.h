#ifndef _CONTROLLERS_H_
#define _CONTROLLERS_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "em_gpio.h"

#include "spi.h"
#include "meta_data.h"

/// Mask for pins 9-12 on PORT B
#define CTRL_PORT_B_MASK 0b1111000000000

/// Mask for pins 0-4 on PORT E
#define CTRL_PORT_E_MASK 0b0000000011111

typedef struct  {
    int id;
    bool enabled;
    int8_t joyDir;
    int8_t prevJoyDir;
    bool joyBtn;
    bool btn1;
    bool btn2;
} Controller;


Controller* CONTROLLER_INPUTS;

Controller* get_next_active_controller(bool reset);


void refresh_controller_status();

uint8_t get_num_active_controllers();

bool check_controller_connection(int id);


/**************************************************************
 * Chip Active Pins
 * ------------------------------------------------------------
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


/**************************************************************
 * Sets up the Chip Select line corresponding to the controller
 * 
 * @param id The ID of the controller to select
 **************************************************************/ 
void select_controller(int id);


/*************************************************************
 * Initializes Controller structs for all active controllers.
 *************************************************************/
void initialize_controllers();


/*************************************************************
 * Polls all connected controllers in turn.
 *************************************************************/
void poll_controllers();


/*************************************************************
 * Polls a single controller connected and stores the
 * input data in the corresponding Controller struct.
 * 
 * @param id The identifier of the controller to poll.
 *************************************************************/
void poll_single_controller(int id);


#endif // _CONTROLLERS_H_