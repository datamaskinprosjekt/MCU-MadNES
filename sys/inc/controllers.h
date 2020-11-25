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

/// Boundry whithin which the joystick is considered at rest
#define CONTROLLER_DEADZONE 35

#define PI 3.14159265

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

int get_controllers_status();

bool check_controller_connection(int id);

void select_controller(int id);

void initialize_controllers();

void refresh_controller_status();

uint8_t get_num_active_controllers();

Controller* get_next_active_controller(bool reset);

void poll_controllers();

void poll_single_controller(int id);


#endif // _CONTROLLERS_H_