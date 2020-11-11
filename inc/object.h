#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "meta_data.h"

#define WIDTH 640
#define HEIGHT 480

typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS,
    LETTER,
    STAR
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
    bool priority;
    bool enabled;
} Object;

// TODO: Move to own utility file
typedef struct {
    uint16_t x;
    uint16_t y;
} vec2d;


SpriteType shipType;
SpriteType statusType;
SpriteType asteroid1Type;
SpriteType asteroid2Type;
SpriteType asteroid3Type;
SpriteType laserType;
SpriteType letterType;
SpriteType starType;

int objMax;
int shipMax;
int statusMax;
int asteroidMax;
int laserMax;
int letterMax;
int starMax;

Object objects[MAX_OBJS];
//int* dirty_objects;
int dirty_objects[MAX_OBJS];

void init_objects();
void add_dirty_object(Object* obj);
int move_object(Object* obj, int rot, int speed);
int get_rot(Object* obj);
void delete_objects();
void print_object(Object* obj);

#endif