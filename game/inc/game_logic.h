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

int gameOverTicksCnt;

// Number of ticks before the game over text is toggled
int gameOverFlickerTicksCnt;

int pressToStartFlickerCounter;

GameState game_state;
bool sem_game;
bool force_quit;
uint32_t lastTicks;
uint32_t deltaTicks;

int playerNum;
int playerPixelTicks;
int asteroidStartPixelTicks;
int asteroidPixelTicks;
int laserPixelTicks;
int flickerTicks;
int explodeTicks;
int shootTicks;
int rotDelayTicks;
int increaseAsteroidSpeedTicks;
int increaseAsteroidSpeedTicksCnt;
int asteroidPerPlayer;
int laserPerPlayer;
int flickerTotal;

PlayerElem* players;
AsteroidElem* asteroids;
LaserElem* lasers;
LetterGameOverElem* letterGameOver;
LetterLogoElem* letterLogo;
LetterPushToStartElem* letterPushToStart;
ScoreElem* scores;

void game_loop();
void run_logic();

void start_screen();
void start_screen_loop();

void game_over_loop();

void init_game();
void time_handler();
void joystick_handler(int playerIdx, int rot);
void button_fuel_handler(int playerIdx);
void button_shoot_handler(int playerIdx);
void button_start_handler();
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
void clear_all();

void game_interrupt_handler();

#endif