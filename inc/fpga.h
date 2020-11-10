#ifndef _FPGA_H_
#define _FPGA_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_ebi.h"
#include "em_gpio.h"
#include "ebi.h"


typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS_BAR
} SpriteName;

typedef struct {
    SpriteName name;
    uint8_t globalSpriteIdx;
    uint8_t length;
} SpriteType;

typedef struct {
    uint16_t id;
    SpriteType* type;
    uint8_t localSpriteIdx;
    uint16_t xPos;
    uint16_t yPos;
    bool xFlip;
    bool yFlip;
    uint8_t priority;
    bool enabled;
} Object;

typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;


void write_sprite_sheet(uint16_t* sprite_sheet, int size);

void write_tile_sheet();

void write_palette(Color* first_color, int size);

void write_object(Object* obj);

#endif // _FPGA_H_