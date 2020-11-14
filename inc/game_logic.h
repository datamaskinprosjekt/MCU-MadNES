#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include <stdlib.h>
#include "controllers.h"

typedef struct {
    int id;
    int hp;
    bool isHit;
    int flickerDownCnt;
    int laserIdx;
    int laserActiveCnt;
    int pixelTicksCnt;
    int flickerTicksCnt;
    Object* shipObj;
    Object* statusObj;
    Controller* controller;
} player_elem;

typedef struct {
    bool isHit;
    int playerIdx;
    int pixelTicksCnt;
    int explodeTicksCnt;
    Object* asteroidObj;
} asteroid_elem;

typedef struct {
    int playerIdx;
    int pixelTicksCnt;
    Object* laserObj;
} laser_elem;

typedef struct {
    Object* letterObj;
} letter_elem;

bool sem_game;
bool force_quit;
uint32_t lastTicks;
uint32_t deltaTicks;

bool game;
int playerPixelTicks;
int asteroidPixelTicks;
int laserPixelTicks;
int flickerTicks;
int explodeTicks;
int asteroidPerPlayer;
int laserPerPlayer;
int flickerTotal;

player_elem* players;
asteroid_elem* asteroids;
laser_elem* lasers;
letter_elem* letters;

void main_logic();
void run_logic();
void simulate_semaphore();
void test_main();

void init_game();
void time_handler(int ticks);
void joystick_handler(int playerIdx, int rot);
void button_fuel_handler(int ticks, int playerIdx);
void button_shoot_handler(int playerIdx);
void button_restart_handler();
void set_asteroid_pos(asteroid_elem* asteroid);
void set_asteroid_new_player(player_elem* oldPlayer);
int get_asteroid_rotation(asteroid_elem* asteroid);
void flicker_player(int ticks, player_elem* player);
bool check_collision_player(player_elem* player, asteroid_elem* asteroid);
bool check_collision_laser(laser_elem* laser, asteroid_elem* asteroid);
void collision_player(player_elem* player, asteroid_elem* asteroid);
void collision_laser(laser_elem* laser, asteroid_elem* asteroid);
bool check_game_over();
void game_over();
void clear_all();
void test_game_print();
void test_print();

#endif