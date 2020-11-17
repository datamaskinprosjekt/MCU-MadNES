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

// The number of distinct star frames
#define NUM_STAR_TYPES 6

typedef enum {
    ASTEROID,
    SHIP,
    LASER,
    STATUS,
    LETTER_GAME_OVER,
    LETTER_LOGO,
    SCORE,
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

/// An array of SpriteTypes to contain the different asteroid types
SpriteType asteroidTypes[NUM_ASTEROID_TYPES];

// Other SpriteTypes
SpriteType laserType;
SpriteType letterGameOverType;
SpriteType letterLogoType;
SpriteType scoreType;
SpriteType starType;

/// The number of each type of star to spawn
int starTypeNumber[NUM_STAR_TYPES];

// Number of each type of object
int objNum;
int shipNum;
int statusNum;
int asteroidNum;
int laserNum;
int num_letters_game_over;
int num_letters_logo;
int num_letters_push_to_start;
int scoreNum;
int starNum;

// tmp value
int statusObjIdx;

// Array of objects and dirty_objects
Object* objects;
int* dirty_objects;

/// To "Notify" that the objects are initialized
bool objects_initialized;


// Initialization
void init_entity_types();
void init_objects(int playerNum, int asteroidPerPlayer, int laserPerPlayer);

// Set object as dirty
void add_dirty_object(Object* obj);

// Calculations and modifications of objects
int move_object(Object* obj, int rot, int speed);
int get_rot(Object* obj);

// Free objects and dirty_objects
void delete_objects();

#endif