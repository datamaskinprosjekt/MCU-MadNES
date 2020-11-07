#ifndef _FPGA_H_
#define _FPGA_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_ebi.h"
#include "em_gpio.h"
#include "ebi.h"

/* Memory bank 0: 0x80000000 -> 0x83ffffff */
//#define fpgaAddr0 0x80000000
/* Memory bank 1: 0x84000000 -> 0x87ffffff */
//#define fpgaAddr1 0x84000000
/* Memory bank 2: 0x88000000 -> 0x8bffffff */
//#define fpgaAddr2 0x88000000
/* Memory bank 3: 0x8c000000 -> 0x8fffffff */
//#define fpgaAddr3 0x8c000000

typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS_BAR
} spriteName;

typedef struct {
    spriteName name;
    uint8_t globalSpriteIdx;
    uint8_t length;
} spriteType;

typedef struct {
    uint16_t id;
    spriteType* type;
    uint8_t localSpriteIdx;
    uint16_t xPos;
    uint16_t yPos;
    bool xFlip;
    bool yFlip;
    uint8_t priority;
    bool enabled;
} Object;

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

uint16_t* fpgaAddr0;

void write_sprite_sheet(uint16_t* spriteSheet, int size);

void write_tile_sheet();

void write_palette(Color* firstColor, int size);

void write_object(Object* obj);

#endif