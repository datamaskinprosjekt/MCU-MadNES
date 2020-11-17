#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>
#include "meta_data.h"

#define BACKGROUND_R 0
#define BACKGROUND_G 0
#define BACKGROUND_B 0

uint8_t sprite_data[NUM_SPRITES * 256];
uint8_t palette_data[NUM_COLORS * 3];

#endif // _DATA_H_