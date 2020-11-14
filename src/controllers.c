#include "controllers.h"


Controller* CONTROLLER_INPUTS;


int get_controllers()
{
    uint32_t port_E_input = GPIO_PortInGet(gpioPortE);
    uint32_t port_B_input = GPIO_PortInGet(gpioPortB);
    
    int active_controllers = (port_E_input | port_B_input);

    return active_controllers;
}


void initialize_controllers()
{
    // Allocate memory for Controller structs and zero the memory region
    CONTROLLER_INPUTS = malloc(sizeof(Controller)*8);
    memset(CONTROLLER_INPUTS, 0, sizeof(Controller)*8);

    int enabled_controllers = get_controllers();

    // Set up Controller structs for active controllers
    for (int i = 0; i < 8; i++) {
        if (enabled_controllers & (1 << i) != 0) {
            CONTROLLER_INPUTS[i] = (Controller) {
                .id = i,
                .enabled = true,
                .joyDir = 0,
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


void send_to_controller(int id, uint8_t data)
{
    // Set Chip Select
    select_controller(id);

    // Send data
    send_ctrl_SPI(&data);
    
    // Clear Chip Select
    select_controller(-1);
}


void poll_controllers()
{
    for (int i = 0; i < 9; i++) {
        if(CONTROLLER_INPUTS[i].enabled) {
            poll_single_controller(i);
        }
    }
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

    // Recieve data
    uint8_t buttons;
    int8_t joystick_x;
    int8_t joystick_y;

    receive_ctrl_SPI(id, &buttons, &joystick_x, &joystick_y);

    // Decode data
    ctrl->btn1 = buttons & 1;
    ctrl->btn2 = buttons & 2;
    ctrl->joyBtn = buttons & 4;

    uint8_t rot = 0;

    // Get rotation value based on joystick-x, joystick-y
    if      (joystick_x >= 0 && joystick_y >= 0) { rot =  joystick_x;      }
    else if (joystick_x >= 0 && joystick_y  < 0) { rot = -joystick_y + 4;  }
    else if (joystick_x <  0 && joystick_y  < 0) { rot = -joystick_x + 8;  }
    else if (joystick_x <  0 && joystick_y >= 0) { rot =  joystick_y + 12; }

    ctrl->joyDir = rot;
}