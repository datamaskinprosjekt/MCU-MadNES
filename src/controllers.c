#include "controllers.h"

Controller* CONTROLLER_INPUTS;


void setup_controller_gpio()
{
    /// Set CA pins on port E as input
    GPIO_PinModeSet(gpioPortE, 4, gpioModeInput, 1);//CA_1
    GPIO_PinModeSet(gpioPortE, 3, gpioModeInput, 1); //CA_2
    GPIO_PinModeSet(gpioPortE, 2, gpioModeInput, 1); //CA_3
    GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 1); //CA_4
    GPIO_PinModeSet(gpioPortE, 0, gpioModeInput, 1); //CA_5

    /// Set CA pins on port B as input
    GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 1); //CA_6
    GPIO_PinModeSet(gpioPortB, 9 , gpioModeInput, 1); //CA_7
    GPIO_PinModeSet(gpioPortB, 12, gpioModeInput, 1); //CA_8
    GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 1); //CA_9

    /// Set CS pins on port B as output
    //GPIO_DriveModeSet(gpioPortB, gpioDriveModeLow);
    GPIO_PinModeSet(gpioPortB, 2, gpioModePushPull, 1); //CS_1
    GPIO_PinModeSet(gpioPortB, 1, gpioModePushPull, 1); //CS_2
    GPIO_PinModeSet(gpioPortB, 0, gpioModePushPull, 1); //CS_3

    /// Set CS pins on port A as output
    GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 1); //CS_4
    GPIO_PinModeSet(gpioPortA, 12, gpioModePushPull, 1); //CS_5
    GPIO_PinModeSet(gpioPortA, 11, gpioModePushPull, 1); //CS_6
    GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 1); //CS_7
    GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull,  1); //CS_8
    GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull,  1); //CS_9

    //GPIO_DriveModeSet(gpioPortE, gpioDriveModeLow);
    //GPIO_PinModeSet(gpioPortE, 5, gpioModePushPull, 0); //CLCK
    //GPIO_PinModeSet(gpioPortE, 6, gpioModeInput, 0); //MISO
    //GPIO_PinModeSet(gpioPortE, 7, gpioModePushPull, 0); //MOSI

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

    return inputs;
}

void send_to_controller(int id)
{
    select_controller(id);

    uint8_t buffer = 0;
    send_ctrl_SPI(&buffer);

    //select_controller(-1);
}

void poll_single_controller(int id)
{
    Controller* ctrl = &CONTROLLER_INPUTS[id];

    select_controller(id);

    uint8_t buffer = 0;
    receive_ctrl_SPI(&buffer);

    select_controller(-1);

    Controller decoded_inputs = decode_controller_frame(buffer);

    decoded_inputs.id = id;
    decoded_inputs.enabled = true;

    *ctrl = decoded_inputs;
}