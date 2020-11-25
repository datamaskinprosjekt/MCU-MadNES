#include "controllers.h"


/**************************************************************
 * Controller Interfacing
 * ------------------------------------------------------------
 * This file contains all the functions needed to handle
 * interfacing with the controllers.
 * 
 * See spi.c for specifics on how data-transfers are handled.
 *************************************************************/

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
 * CS4 = PA13
 * CS5 = PA12
 * CS6 = PA11
 * CS7 = PA10
 * CS8 = PA9
 * CS9 = PA8
 **************************************************************/


/*************************************************************
 * Gets the active controllers based on the Chip Available pins
 *************************************************************/
int get_controllers_status()
{
    uint32_t port_E_input = GPIO_PortInGet(gpioPortE);
    uint32_t port_B_input = GPIO_PortInGet(gpioPortB);
    
    int active_controllers = (port_E_input | port_B_input);

    return active_controllers;
}


/*************************************************************
 * Checks if a spesific controller is connected.
 * 
 * @param id The identifier of the controller to check for connection.
 * @returns Boolean to indicate if the controller is connected. 
 *************************************************************/
bool check_controller_connection(int id)
{

    bool retVal = false;
    
    switch(id) {
        case 0:
            retVal = GPIO_PinInGet(gpioPortE, 4) == 0;
            break;
        case 1:
            retVal = GPIO_PinInGet(gpioPortE, 3) == 0;
            break;
        case 2:
            retVal = GPIO_PinInGet(gpioPortE, 2) == 0;
            break;
        case 3:
            retVal = GPIO_PinInGet(gpioPortE, 1) == 0;
            break;
        case 4:
            retVal = GPIO_PinInGet(gpioPortE, 0) == 0;
            break;
        case 5:
            retVal = GPIO_PinInGet(gpioPortB, 10) == 0;
            break;
        case 6:
            retVal = GPIO_PinInGet(gpioPortB, 9) == 0;
            break;
        case 7:
            retVal = GPIO_PinInGet(gpioPortB, 12) == 0;
            break;
        default:
            retVal = false;
            break;
    }

    return retVal;
}


/**************************************************************
 * Sets the Chip Select line corresponding to a specific controller.
 * 
 * @param id The identifier of the controller to select.
 **************************************************************/ 
void select_controller(int id)
{
    if(id == -1) {
        GPIO_PortOutSetVal(gpioPortB, 0, 0b0000000000000111); // Set correct port
        GPIO_PortOutSetVal(gpioPortA, 0, 0b0000000111111000); // Clear other port
    }
    if (id <= 2) {
        GPIO_PortOutSetVal(gpioPortB, 1 << (2 - id), 0b0000000000000111); // Set correct port
        GPIO_PortOutSetVal(gpioPortA, 0, 0b0000000111111000); // Clear other port
    } else {
        GPIO_PortOutSetVal(gpioPortA, 1 << (16 - id), 0b0000000111111000); // Set correct port
        GPIO_PortOutSetVal(gpioPortB, 0, 0b0000000000000111); // Clear other port
    }
}


/*************************************************************
 * Allocates memory for Controller structures for
 * the maximum number of controllers.
 *************************************************************/
void initialize_controllers()
{
    CONTROLLER_INPUTS = (Controller*) malloc(sizeof(Controller) * 8);
    memset(CONTROLLER_INPUTS, 0, sizeof(Controller) * 8);
}


/*************************************************************
 * Resets the data of the Controller structures.
 *************************************************************/
void refresh_controller_status()
{
    bool is_avail;

    for(int i = 0; i < 8; i++)
    {
        is_avail = check_controller_connection(i);

        if(is_avail) {
            CONTROLLER_INPUTS[i] = (Controller) {
                .id = i,
                .enabled = true,
                .joyDir = -1,
                .prevJoyDir = -1,
                .joyBtn = false,
                .btn1 = false,
                .btn2 = false   
            };
        } else {
            CONTROLLER_INPUTS[i] = (Controller) {
                .id = i,
                .enabled = false,
                .joyDir = -1,
                .prevJoyDir = -1,
                .joyBtn = false,
                .btn1 = false,
                .btn2 = false
            };
        }

    }
}


/*************************************************************
 * Gets the number of controllers connected.
 * 
 * @returns The number of active controllers.
 *************************************************************/
uint8_t get_num_active_controllers()
{
    uint8_t counter = 0;

    for(int i = 0; i < 8; i++ )
    {

        if(CONTROLLER_INPUTS[i].enabled) {
            counter++;
        }

    }

    return counter;
}


/*************************************************************
 * Gets the first active controller using the Controller structure.
 * 
 * @param reset
 * @returns A pointer to the Controller structure of the first active controller.
 *************************************************************/
Controller* get_next_active_controller(bool reset)
{
    static int i = -1;

    if(reset) {
        i = -1;
        return;
    }

    i += 1;

    while(!CONTROLLER_INPUTS[i].enabled && i < MAX_CONTROLLERS)
    {
       i++; 
    }

    if(i >= MAX_CONTROLLERS) return NULL;

    return &CONTROLLER_INPUTS[i];
}


/*************************************************************
 * Polls all connected controllers in turn.
 *************************************************************/
void poll_controllers()
{
    for (int i = 0; i < 9; i++) {
        if(CONTROLLER_INPUTS[i].enabled) {
            poll_single_controller(i);
        }
    }
}


/*************************************************************
 * @brief Polls a single controller connected and stores the
 * input data in the corresponding Controller struct.
 * -----------------------------------------------------------
 * Receives data from the buttons and the joystick of a
 * controller using the SPI and decodes the recevied data.
 * 
 * The buttons are considered active (1) or not active (0).
 * Button Bitmask
 * 00000001: A
 * 00000010: B
 * 00000100: Joystick Button
 * 
 * The joystick data is translated into a rotation.
 * Joystick rotations
 * -1: no rotation
 * 0 to 16: 0 to 360 degrees
 * 
 * @param id The identifier of the controller to poll.
 *************************************************************/
void poll_single_controller(int id)
{
    Controller* ctrl = &CONTROLLER_INPUTS[id];

    bool avail = check_controller_connection(id);

    if(!avail) {
        ctrl->btn1 = false;
        ctrl->btn2 = false;
        ctrl->joyBtn = false;
        ctrl->joyDir = -1;
        return;
    }

    // Recieve data
    uint8_t buttons;
    uint8_t joystick_x;
    uint8_t joystick_y;

    receive_ctrl_SPI(id, &buttons, &joystick_x, &joystick_y);
    
    // Decode data
    ctrl->btn1 =   buttons & 1;
    ctrl->btn2 =   buttons >> 1 & 1;
    ctrl->joyBtn = buttons >> 2 & 1;

    // Save the last state for smoother interpolation between states
    ctrl->prevJoyDir = ctrl->joyDir;

    if( (abs(127 - joystick_x) < CONTROLLER_DEADZONE) && (abs(127 - joystick_y) < CONTROLLER_DEADZONE)) {
        ctrl->joyDir = -1;
        return;
    }

    int16_t vertical = joystick_y - 127;
    int16_t horizontal = joystick_x - 127;

    uint16_t angle = (uint16_t) (round((atan2(-horizontal, -vertical)+PI)*180/PI));

    if(angle < 0) {
        angle = 360 - angle;
    }

    bool centered = ((vertical < 5) && (vertical > -5) && (horizontal < 5) && (horizontal > -5));

    uint8_t joyDir = round( (long) ((angle - 0) * (15 - 0)) / (long) (360 - 0) + 0);

    if(centered) {
        ctrl->joyDir = -1;
    } else {
        ctrl->joyDir = joyDir;
    }
}