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

SpriteType ship1Type;
SpriteType ship2Type;
SpriteType ship3Type;
SpriteType ship4Type;
SpriteType ship5Type;
SpriteType ship6Type;
SpriteType ship7Type;
SpriteType ship8Type;
SpriteType ship9Type;
SpriteType status1Type;
SpriteType status2Type;
SpriteType status3Type;
SpriteType status4Type;
SpriteType status5Type;
SpriteType status6Type;
SpriteType status7Type;
SpriteType status8Type;
SpriteType status9Type;
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

Object* objects;
//int* dirty_objects;
int* dirty_objects;

void init_objects();
void add_dirty_object(Object* obj);
int move_object(Object* obj, int rot, int speed);
int get_rot(Object* obj);
void delete_objects();
void print_object(Object* obj);

#endif