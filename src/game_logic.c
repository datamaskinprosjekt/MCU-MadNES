#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "object.h"
#include "game_logic.h"

int main(void) {
	game = 1;

	init_game();
	test_print();
	while (game) {
		game = 0;
		button_handler();
		sleep(1);
	}
	test_print();

	end_game();

	return 0;
}

void init_game() {
	playerSpeed = 15;
	asteroidSpeed = 10;
	laserSpeed = 30;
	asteroidCnt = asteroidMax;
	laserCnt = 0;

	init_objects();

	players = (player_elem *) malloc(sizeof(player_elem) * shipMax);
	asteroids = (asteroid_elem *) malloc(sizeof(asteroid_elem) * asteroidMax);

	for (int i=0; i<shipMax; i++) {
		players[i] = (player_elem) {3, &shipObjs[i], &statusObjs[i]};
	}
	for (int i=0; i<asteroidMax; i++) {
		asteroids[i] = (asteroid_elem) {asteroidSpeed, 0, &asteroidObjs[i]};
	}
}

void time_handler() {	
	for (int i=0; i<asteroidMax; i++) {
		asteroid_elem* asteroid = &asteroids[i];
		if (asteroid->asteroidObj->enable) {
			if (asteroid->isHit) {
				if (asteroid->asteroidObj->localSpriteIdx + 1 >= asteroid->asteroidObj->type->length) {
					asteroid->asteroidObj->enable = 0;
				} else {
					asteroid->asteroidObj->localSpriteIdx++;
				}
			} else {
				// move asteroid

				// ship collision
				if (1) {
					collision_player(&players[0], asteroid);
					add_dirty_object(players[0].statusObj);
					add_dirty_object(asteroid->asteroidObj);
				}
			}

			for (int j=0; j<laserMax; j++) {
				object* laserObj = &laserObjs[i];
				// laser collision
				if (1) {
					collision_laser(laserObj, asteroid);
				}
			}

			add_dirty_object(asteroid->asteroidObj);
		}
	}

	for (int i=0; i<laserMax; i++) {
		object* laserObj = &laserObjs[i];
		if (laserObj->enable) {
			// move laser
			add_dirty_object(laserObj);
		}
	}

	// Check timer and asteroidCnt < asteroidMax
	// enable asteroid
	// reset values

	add_dirty_object(players[0].statusObj);
}

void joystick_handler(int rot) {
	int dir = rot / 4;
	int spriteIdx = rot % 4;

	players[0].shipObj->localSpriteIdx = rot;
	// Move spaceship

	for (int i=0; i<asteroidMax; i++) {
		asteroid_elem* asteroid = &asteroids[i];
		// Detect collision
		if (1) {
			collision_player(&players[0], asteroid);
			add_dirty_object(players[0].statusObj);
			add_dirty_object(asteroid->asteroidObj);
		}
	}
	
	add_dirty_object(players[0].shipObj);
}

void button_handler() {
	if (laserCnt < laserMax) {
		for (int i=0; i<laserMax; i++) {
			object* laserObj = &laserObjs[i];
			if (laserObj->enable == 0) {
				laserObj->enable = 1;
				laserObj->localSpriteIdx = players[0].shipObj->localSpriteIdx;
				laserObj->xPos = players[0].shipObj->xPos;
				laserObj->yPos = players[0].shipObj->yPos;
				laserObj->xFlip = players[0].shipObj->xFlip;
				laserObj->yFlip = players[0].shipObj->yFlip;
				add_dirty_object(laserObj);
				break;
			}
		}
	}
}

void collision_player(player_elem* player, asteroid_elem* asteroid) {
	if (player->hp == 0) {
		game = 0;
	} else {
		player->hp--;
		player->statusObj->localSpriteIdx--;
		asteroid->asteroidObj->enable = 0;
	}
}

void collision_laser(object* laserObj, asteroid_elem* asteroid) {
	laserObj->enable = 0;
	asteroid->isHit = 1;
	asteroid->asteroidObj->localSpriteIdx++;
}

void end_game() {
	delete_objects();
	free(players);
	free(asteroids);
}

void test_print() {
	if (players[0].shipObj->type->name == SHIP) {
		printf("player %d\n", players[0].shipObj->id);
		print_object(players[0].shipObj);
		print_object(players[0].statusObj);
		printf("\n");
	}

	printf("asteroids\n");
	for (int i=0; i<asteroidMax; i++) {
		print_object(&asteroidObjs[i]);
	}
	printf("\n");

	printf("lasers\n");
	for (int i=0; i<laserMax; i++) {
		print_object(&laserObjs[i]);
	}
	printf("\n\n");
}
