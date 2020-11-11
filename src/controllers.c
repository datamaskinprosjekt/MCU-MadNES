#include "controllers.h"

extern Controller* CONTROLLER_INPUTS;


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

    /// Set CS pins on board A as output
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