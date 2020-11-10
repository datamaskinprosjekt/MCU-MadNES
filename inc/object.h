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
} SpriteType;

typedef struct {
    uint16_t id;
    SpriteType* type;
    uint8_t localSpriteIdx;
    uint16_t xPos;
    uint16_t yPos;
    bool xFlip;
    bool yFlip;
    bool enable;
    bool priority;
} Object;

// TODO: Move to own utility file
typedef struct {
    uint16_t x;
    uint16_t y;
} vec2d;

// TODO: Move to own Module
typedef struct {
    uint16_t width;
    uint16_t height;
    Object* boundObject; 
    vec2d anchor;
} CollisionBox2D;

typedef struct {
    Object* first;
    Object* second;
} CollisionBox2D_ObjectObjectPair;

bool CB2D_Check_Collision(CollisionBox2D* first, CollisionBox2D* second);
vec2d CB2D_Check_Force_Collision(CollisionBox2D* first, CollisionBox2D* second);

CollisionBox2D_ObjectObjectPair* CB2D_Get_All_Collisions();
CollisionBox2D_ObjectObjectForcePair* CB2D_Get_All_Force_Collisions();


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

Object* objects;
int* dirty_objects;

void init_objects();
void add_dirty_object(Object* obj);
int move_object(Object* obj, int rot, int speed);
int get_rot(Object* obj);
void delete_objects();
void print_object(Object* obj);

#endif