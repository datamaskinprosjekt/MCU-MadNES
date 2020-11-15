#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "meta_data.h"

#define WIDTH 640
#define HEIGHT 480

// The number of distinct ship sprites
#define NUM_SHIP_TYPES 9

// The number of distinct status bar sprites
#define NUM_STATUS_TYPES 9

// The number of distinct asteroid sprites
#define NUM_ASTEROID_TYPES 3

// The number of distinct star sprites
#define NUM_STAR_TYPES 6


//#define DEBUG_WRITE_OBJECT_DIRECT true

typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS,
    LETTER_GAME_OVER,
    LETTER_LOGO,
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


/// An array of SpriteTypes to contain the different ship types
SpriteType shipTypes[NUM_SHIP_TYPES];

/// An array of SpriteTypes to contain the status indicators for the different players
SpriteType statusTypes[NUM_SHIP_TYPES];

SpriteType asteroidTypes[NUM_ASTEROID_TYPES];

SpriteType laserType;
SpriteType letterGameOverType;
SpriteType letterLogoType;
SpriteType starType;

/// The number of different types of stars to spawn
int starTypeNumber[NUM_STAR_TYPES];

int objNum;
int shipNum;
int statusNum;
int asteroidNum;
int laserNum;
int num_letters_game_over;
int num_letters_logo;
int num_letters_push_to_start;


int starNum;

Object* objects;
int* dirty_objects;

bool objects_initialized;

/**************************************************************
 * Initializes SpriteType objects 
 * @see struct SpriteType
 **************************************************************/
void init_entity_types();

void init_objects(int playerNum, int asteroidPerPlayer, int laserPerPlayer);


/**************************************************************
 * Marks the object as dirty.
 **************************************************************/
void add_dirty_object(Object* obj);


/**************************************************************
 * Calculates and updates the position of the object.
 * 
 * @param obj The object to move.
 * @param rot The rotation to consider.
 * @param speed The speed to consider.
 **************************************************************/
int move_object(Object* obj, int rot, int speed);
int get_rot(Object* obj);
void delete_objects();
void print_object(Object* obj);

#endif