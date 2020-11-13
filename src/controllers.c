#include "controllers.h"


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


Controller decode_controller_frame(uint8_t frame)
{
    Controller inputs;

    // Frame format:
    // Bits 6-3: Potentiometer 
    // Bit  2: Joy Btn
    // Bit  1: Btn 1
    // Bit  0: Btn 2

    inputs.joyDir = (frame & 0b01111000);
    inputs.joyBtn = (frame & 0b00000100) > 0;
    inputs.btn1   = (frame & 0b00000010) > 0;
    inputs.btn2   = (frame & 0b00000001) > 0;

    if(frame & 0b10000000 > 0) {
        inputs.joyDir = -1;
    }

    return inputs;
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


void poll_single_controller(int id)
{
    Controller* ctrl = &CONTROLLER_INPUTS[id];

    // Set Chip Select
    select_controller(id);

    // Recieve data
    uint8_t buffer = 0;
    receive_ctrl_SPI(&buffer);

    // Clear Chip Select
    select_controller(-1);

    // Decode data
    Controller decoded_inputs = decode_controller_frame(buffer);

    decoded_inputs.id = id;
    decoded_inputs.enabled = true;

    *ctrl = decoded_inputs;
}