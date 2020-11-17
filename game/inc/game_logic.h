#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define GAME_OVER_DURATION 70000

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

#include "sys_time.h"
#include "controllers.h"
#include "interrupt_handlers.h"
#include "object.h"
#include "sound.h"
#include "meta_data.h"

typedef struct {
    int id;
    int hp;
    bool isHit;
    int flickerDownCnt;
    int laserIdx;
    int localLaserNext;
    int pixelTicksCnt;
    int shootTicksCnt;
    int flickerTicksCnt;
    Object* shipObj;
    Object* statusObj;
    Controller* controller;
    bool enabled;
} PlayerElem;

typedef struct {
    bool isHit;
    int playerIdx;
    int pixelTicksCnt;
    int explodeTicksCnt;
    bool prevCollideState;
    Object* asteroidObj;
} AsteroidElem;

typedef struct {
    int playerIdx;
    int pixelTicksCnt;
    Object* laserObj;
} LaserElem;

typedef struct {
    Object* letterGameOverObj;
} LetterGameOverElem;

typedef struct {
    Object* letterLogoObj;
} LetterLogoElem;

typedef struct {
    Object* letterPushToStartObj;
} LetterPushToStartElem;

typedef struct {
    Object* scoreObj;
} ScoreElem;

typedef enum {
    GAME_START_SCREEN,
    GAME_RUN,
    GAME_OVER
} GameState;

// State values
GameState game_state;
bool sem_game;
bool force_quit;
// Ticks values
uint32_t lastTicks;
uint32_t deltaTicks;

// Current ticks count before GAME OVER display is over
int gameOverTicksCnt;
// Current ticks count before the GAME OVER text is flickered
int gameOverFlickerTicksCnt;
// Current ticks count before the PRESS TO PLAY text is flickered
int pressToStartFlickerTicksCnt;

// Ticks before moving each pixel
int playerPixelTicks;
int asteroidStartPixelTicks;
int asteroidPixelTicks;
int laserPixelTicks;

// Ticks before ship flicker
int flickerTicks;
// Ticks before next explode frame
int explodeTicks;
// Ticks before player can shoot
int shootTicks;

// Ticks before increasing asteroid speed
int increaseAsteroidSpeedTicks;
int increaseAsteroidSpeedTicksCnt;

// Number of elements
int playerNum;
int asteroidPerPlayer;
int laserPerPlayer;

// Number of flickers for ship
int flickerTotal;

// Arrays of elements
PlayerElem* players;
AsteroidElem* asteroids;
LaserElem* lasers;
LetterGameOverElem* letterGameOver;
LetterLogoElem* letterLogo;
LetterPushToStartElem* letterPushToStart;
ScoreElem* scores;

// Game body functions
void game_loop();
void init_game();
void run_logic();
void clear_all();

// Game state functions
void start_screen();
void start_screen_loop();
void game_over_loop();

// Game frame action functions
void time_handler();
void joystick_handler(int playerIdx, int rot);
void button_fuel_handler(int playerIdx);
void button_shoot_handler(int playerIdx);
void button_start_handler();

// Game logic functions
void set_asteroid_pos(AsteroidElem* asteroid);
void set_asteroid_new_player(PlayerElem* oldPlayer);
int get_asteroid_rotation(AsteroidElem* asteroid);
void flicker_player(PlayerElem* player);
bool check_collision_player(PlayerElem* player, AsteroidElem* asteroid);
bool check_collision_asteroid(AsteroidElem* asteroid1, AsteroidElem* asteroid2);
bool check_collision_laser(LaserElem* laser, AsteroidElem* asteroid);
void collision_player(PlayerElem* player, AsteroidElem* asteroid);
void collision_laser(LaserElem* laser, AsteroidElem* asteroid);
bool check_game_over();
void game_over();

// Game interrupt handler
void game_interrupt_handler();

#endif