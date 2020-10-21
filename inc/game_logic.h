#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t speed;
    bool isHit;
    object* asteroidObj;
} asteroid_elem;

typedef struct {
    uint8_t hp;
    object* shipObj;
    object* statusObj;
} player_elem;

//uniontype and typepunning

bool game;
int playerSpeed;
int asteroidSpeed;
int laserSpeed;
int asteroidCnt;
int laserCnt;
player_elem* players;
asteroid_elem* asteroids;

void init_game();
void time_handler();
void button_handler();
void joystick_handler(int rot);
void collision_player(player_elem* player, asteroid_elem* asteroid);
void collision_laser(object* laserObj, asteroid_elem* asteroid);
void end_game();
void test_print();

#endif