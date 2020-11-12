#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t id;
    uint8_t hp;
    bool isHit;
    uint8_t flickerDownCnt;
    uint8_t laserIdx;
    uint8_t laserActiveCnt;
    object* shipObj;
    object* statusObj;
} player_elem;

typedef struct {
    bool isHit;
    int playerIdx;
    object* asteroidObj;
} asteroid_elem;

typedef struct {
    int playerIdx;
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
int flickerCnt;
int laserCnt;

player_elem* players;
asteroid_elem* asteroids;
laser_elem* lasers;
letter_elem* letters;

void init_game();
void time_handler(int ticks);
void joystick_handler(int playerIdx, int rot);
void button_fuel_handler(int playerIdx);
void button_shoot_handler(int playerIdx);
void button_restart_handler();
void set_asteroid_pos(asteroid_elem* asteroid);
void set_asteroid_new_player(player_elem* oldPlayer);
int get_asteroid_rotation(asteroid_elem* asteroid);
void flicker_player(player_elem* player);
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