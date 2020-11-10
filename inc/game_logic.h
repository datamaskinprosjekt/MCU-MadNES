#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t hp;
    object* shipObj;
    object* statusObj;
} player_elem;

typedef struct {
    uint8_t speed;
    bool isHit;
    object* asteroidObj;
} asteroid_elem;

typedef struct {
    object* laserObj;
} laser_elem;

typedef struct {
    object* letterObj;
} letter_elem;

int roundNo;
bool game;
int playerSpeed;
int asteroidSpeed;
int laserSpeed;
int asteroidCnt;
int laserCnt;

player_elem* players;
asteroid_elem* asteroids;
laser_elem* lasers;
letter_elem* letters;

void init_game();
void time_handler();
void joystick_handler(int rot);
void button_fuel_handler();
void button_shoot_handler();
void button_restart_handler();
bool check_collision_player(player_elem* player, asteroid_elem* asteroid);
bool check_collision_laser(laser_elem* laser, asteroid_elem* asteroid);
void collision_player(player_elem* player, asteroid_elem* asteroid);
void collision_laser(laser_elem* laser, asteroid_elem* asteroid);
void end_game();
void test_game_print();
void test_print();

#endif