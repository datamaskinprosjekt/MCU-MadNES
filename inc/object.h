#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS_ELEMENT
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
    uint8_t state; //0000 [flipY][flipX][priority][enable]
} Object;

spriteType astroid = (spriteType) {ASTEROID, 0, 11};
spriteType ship = (spriteType) {SHIP, 11, 5};
spriteType laser = (spriteType) {LASER, 16, 5};
spriteType statusElement = (spriteType) {STATUS_ELEMENT, 21, 3};

void init_object();

void write_object(Object* obj);

#endif