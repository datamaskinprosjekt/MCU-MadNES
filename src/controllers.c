#include "controllers.h"

Controller* CONTROLLER_INPUTS;


void setup_controller_gpio()
{
    /// Set CA pins on port E as input
    GPIO_PinModeSet(gpioPortE, 4, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortE, 3, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortE, 2, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortE, 1, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortE, 0, gpioModeInputPull, 1);

    /// Set CA pins on port B as input
    GPIO_PinModeSet(gpioPortB, 10, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortB, 9, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortB, 12, gpioModeInputPull, 1);
    GPIO_PinModeSet(gpioPortB, 11, gpioModeInputPull, 1);

    /// Set CS pins on port B as output
    GPIO_PinModeSet(gpioPortB, 2, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortB, 1, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortB, 0, gpioModePushPull, 0);

    /// Set CS pins on port A as output
    GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortA, 12, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortA, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull, 0);
}


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

Controller decode_controller_frame(uint8_t frame)
{
    Controller inputs;

    // Frame format:
    // Bits 0-4: Potentiometer 
    // Bit  5: Joy Btn
    // Bit  6: Btn 1
    // Bit  7: Btn 2

    inputs.joyDir = (frame & 0b00011111);
    inputs.joyBtn = (frame & 0b00100000) > 0;
    inputs.btn1   = (frame & 0b01000000) > 0;
    inputs.btn2   = (frame & 0b10000000) > 0;

    return inputs;
}

void poll_single_controller(int id)
{
    Controller* ctrl = &CONTROLLER_INPUTS[id];

    select_controller(id);

    uint8_t buffer = 0;
    receive_ctrl_SPI(&buffer);

    Controller decoded_inputs = decode_controller_frame(buffer);

    decoded_inputs.id = id;
    decoded_inputs.enabled = true;

    *ctrl = decoded_inputs;
}