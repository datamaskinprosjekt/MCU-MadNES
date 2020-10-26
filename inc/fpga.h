#ifndef _FPGA_H_
#define _FPGA_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_ebi.h"

/* Memory bank 0: 0x80000000 -> 0x83ffffff */
#define fpgaAddr0 0x80000000
/* Memory bank 1: 0x84000000 -> 0x87ffffff */
#define fpgaAddr1 0x84000000
/* Memory bank 2: 0x88000000 -> 0x8bffffff */
#define fpgaAddr2 0x88000000
/* Memory bank 3: 0x8c000000 -> 0x8fffffff */
#define fpgaAddr3 0x8c000000

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
    bool enabled;
    uint8_t priority;
} Object;

/* Write the initial data to the fpga */
void write_initial_data();

void write_sprite_sheet();

void write_tile_sheet();

void write_palettes();

/* Write all objects to fpga */
void write_object(Object* obj);

void write_background();

void handle_fpga_interrupt();

/* Write a new current pallette to the fpga */
void update_palette(uint8_t paletteId);

#endif