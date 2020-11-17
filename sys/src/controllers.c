#include "controllers.h"

#define CONTROLLER_DEADZONE 35

/**************************************************************
 * Controller Interfacing.
 * ------------------------------------------------------------
 * This file contains all the functions needed to handle
 * interfacing with the controllers.
 * 
 * See spi.c for specifics on how data-transfers are handled.
 *************************************************************/

#define PI 3.14159265


int get_controllers()
{
    uint32_t port_E_input = GPIO_PortInGet(gpioPortE);
    uint32_t port_B_input = GPIO_PortInGet(gpioPortB);
    
    int active_controllers = (port_E_input | port_B_input);

    return active_controllers;
}


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


// Todo: Comment this function
void initialize_controllers()
{
    // Allocate memory for Controller structs and zero the memory region
    CONTROLLER_INPUTS = (Controller*) malloc(sizeof(Controller) * 8);
    memset(CONTROLLER_INPUTS, 0, sizeof(Controller) * 8);
}

// TODO: Comment this function
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


void poll_controllers()
{
    for (int i = 0; i < 9; i++) {
        if(CONTROLLER_INPUTS[i].enabled) {
            poll_single_controller(i);
        }
    }
}

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


/*************************************************************
 * Uses the SPI bus to poll a single controller for its inputs
 * and decodes the rotation value for the joystick
 *
 * Button Bitmask:
 * 00000001: A
 * 00000010: B
 * 00000100: Joystick Button 
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



    //if(joystick_x > 80 && joystick_x < 160 && joystick_y == 255) ctrl->joyDir = 0;
    //else if(joystick_x == 255 && joystick_y == 255) ctrl->joyDir = 2;
    //else if(joystick_x == 255 && joystick_y > 150 ) ctrl->joyDir = 4;
    //else if(joystick_x == 255 && joystick_y < 20 )  ctrl->joyDir = 6;
    //else if(joystick_x > 80 && joystick_x < 160 && joystick_y < 20 )  ctrl->joyDir = 8;
    //else if(joystick_x < 20 && joystick_y < 20 )  ctrl->joyDir = 12;
    //else if(joystick_x < 80 && joystick_y < 20 )  ctrl->joyDir = 10;
    //else if(joystick_x < 20 && joystick_y > 80 && joystick_y < 160) ctrl->joyDir = 14;

    // Values to map from 
    // 0 deg:   (131, 255)
    // 45 deg:  (255, 255) 
    // 90 deg:  (255, 126)
    // 135 deg: (255, 0)
    // 180 deg: (131, 0)
    // 225 deg: (0  , 0)
    // 270 deg: (0  , 126)
    // 315 deg: (255, 0)

}