#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define GAME_OVER_DURATION 70000

#include <stdint.h>
#include <stdlib.h>
#include "controllers.h"

typedef struct {
    int id;
    int hp;
    bool isHit;
    int score;
    int flickerDownCnt;
    int laserIdx;
    int localLaserNext;
    int pixelTicksCnt;
    int shootTicksCnt;
    int rotDelayTicksCnt;
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

int gameOverCounter;

// Number of ticks before the game over text is toggled
int gameOverFlickerCounter;

int pressToStartFlickerCounter;

GameState game_state;
bool sem_game;
bool force_quit;
uint32_t lastTicks;
uint32_t deltaTicks;

int playerNum;
int playerPixelTicks;
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
void simulate_semaphore();
void test_main();

void start_screen();
void start_screen_loop();

void game_over_loop();

void init_game();
void time_handler(int deltaTicks);
void joystick_handler(int playerIdx, int rot);
void button_fuel_handler(int ticks, int playerIdx);
void button_shoot_handler(int deltaTicks, int playerIdx);
void button_start_handler();
void set_asteroid_pos(AsteroidElem* asteroid);
void set_asteroid_new_player(PlayerElem* oldPlayer);
int get_asteroid_rotation(AsteroidElem* asteroid);
void flicker_player(int deltaTicks, PlayerElem* player);
bool check_collision_player(PlayerElem* player, AsteroidElem* asteroid);
bool check_collision_asteroid(AsteroidElem* asteroid1, AsteroidElem* asteroid2);
bool check_collision_laser(LaserElem* laser, AsteroidElem* asteroid);
void collision_player(PlayerElem* player, AsteroidElem* asteroid);
void collision_laser(LaserElem* laser, AsteroidElem* asteroid);
bool check_game_over();
void game_over();
void clear_all();
void test_game_print();
void test_print();

#endif