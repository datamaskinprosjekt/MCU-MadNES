#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 640
#define HEIGHT 480

typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS
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
    bool enable;
    uint8_t priority; //0000 [flipY][flipX][priority][enable]
} object;

spriteType shipType;
spriteType statusType;
spriteType asteroidType;
spriteType laserType;

int objMax;
int shipMax;
int statusMax;
int asteroidMax;
int laserMax;

object* objs;
object* dirtyObjs;

void init_objects();
void add_dirty_object(object* obj);
int get_rot(object* obj);
void delete_objects();
void print_object(object* obj);

#endif