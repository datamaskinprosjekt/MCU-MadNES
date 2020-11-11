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
    STATUS,
    LETTER,
    STAR
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
    bool priority;
} object;

spriteType ship1Type;
spriteType ship2Type;
spriteType statusType;
spriteType asteroid1Type;
spriteType asteroid2Type;
spriteType asteroid3Type;
spriteType laserType;
spriteType letterType;
spriteType starType;

int objMax;
int shipMax;
int statusMax;
int asteroidMax;
int laserMax;
int letterMax;
int starMax;

object* objs;
int* dirtyObjs;

void init_objects();
void add_dirty_object(object* obj);
int move_object(object* obj, int rot, int speed);
int get_rot(object* obj);
void delete_objects();
void print_object(object* obj);

#endif