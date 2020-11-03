#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "object.h"
#include "game_logic.h"

int main(void) {
	game = 1;

	init_game();
	test_print();
	while (game) {
		game = 0;
		button_handler();
		time_handler();
		//joystick_handler(0);
	}
	test_print();

	end_game();

	return 0;
}

void init_game() {
	playerSpeed = 2;
	asteroidSpeed = 2;
	laserSpeed = 5;
	asteroidCnt = asteroidMax;
	laserCnt = 0;

	init_objects();

	players = (player_elem *) malloc(sizeof(player_elem) * shipMax);
	asteroids = (asteroid_elem *) malloc(sizeof(asteroid_elem) * asteroidMax);
	lasers = (laser_elem *) malloc(sizeof(laser_elem) * laserMax);

	int cnt = 0;
	for (int i=0; i<shipMax; i++) {
		players[i] = (player_elem) {3, &objs[i], &objs[shipMax + i]};
	}
	cnt += shipMax + statusMax;
	for (int i=0; i<asteroidMax; i++) {
		asteroids[i] = (asteroid_elem) {asteroidSpeed, 0, &objs[cnt + i]};
	}
	cnt += asteroidMax;
	for (int i=0; i<laserMax; i++) {
		lasers[i] = (laser_elem) {&objs[cnt + i]};
	}
}

void time_handler() {	
	for (int i=0; i<asteroidMax; i++) {
		asteroid_elem* asteroid = &asteroids[i];
		if (asteroid->asteroidObj->enable) {
			if (asteroid->isHit) {
				if (asteroid->asteroidObj->localSpriteIdx + 1 >= asteroid->asteroidObj->type->length) {
					asteroid->asteroidObj->enable = 0;
					asteroidCnt--;
				} else {
					asteroid->asteroidObj->localSpriteIdx++;
				}
			} else {
				int xDiff = asteroid->asteroidObj->xPos - players[0].shipObj->xPos;
				int yDiff = asteroid->asteroidObj->yPos - players[0].shipObj->yPos;
				if (xDiff != 0 || yDiff != 0) {
					int rot;
					int xDiffAbs = abs(xDiff);
					int yDiffAbs = abs(yDiff);
					if (xDiffAbs < yDiffAbs)
					{
						float tmp = (float) xDiffAbs / yDiffAbs;
						if (tmp < 0.25) {
							rot = 0;
						} else if (tmp > 0.75) {
							rot = 2;
						} else {
							rot = 1;
						}
					}
					else if(yDiffAbs < xDiffAbs)
					{
						float tmp = (float) yDiffAbs / xDiffAbs;
						if (tmp < 0.25) {
							rot = 4;
						} else if (tmp > 0.75) {
							rot = 2;
						} else {
							rot = 3;
						}
					}
					else
					{
						rot = 2;
					}

					if (xDiff < 0 && yDiff <= 0)
					{
						rot = 8 - rot;
					}
					else if (xDiff >= 0 && yDiff <= 0)
					{
						rot += 8;
					}
					else if (xDiff > 0 && yDiff >= 0)
					{
						rot = (16 - rot) % 16;
					}

					move_object(asteroid->asteroidObj, rot, asteroid->speed);
				}

				if (check_collision_player(&players[0], asteroid)) {
					collision_player(&players[0], asteroid);
					add_dirty_object(players[0].statusObj);
				}

				for (int j=0; j<laserMax; j++) {
					laser_elem* laser = &lasers[j];
					if (laser->laserObj->enable) {
						int laserRot = get_rot(laser->laserObj);
						move_object(laser->laserObj, laserRot, laserSpeed);

						if (check_collision_laser(laser, asteroid)) {
							collision_laser(laser, asteroid);
						}

						add_dirty_object(laser->laserObj);
					}
				}
			}
		} else {
			asteroid->isHit = 0;
			asteroid->asteroidObj->enable = 1;
			asteroid->asteroidObj->localSpriteIdx = asteroid->asteroidObj->type->globalSpriteIdx;
			asteroid->asteroidObj->xPos = 0;
			asteroid->asteroidObj->yPos = 0;
			asteroidCnt++;
		}
		add_dirty_object(asteroid->asteroidObj);
	}

	if (!game) {
		// display game over
	}
}

void joystick_handler(int rot) {
	int spriteIdx = rot == 4 ? rot : rot % 4;
	players[0].shipObj->localSpriteIdx = spriteIdx;

	move_object(players[0].shipObj, rot, playerSpeed);

	for (int i=0; i<asteroidMax; i++) {
		asteroid_elem* asteroid = &asteroids[i];
		if (asteroid->asteroidObj->enable) {
			if (check_collision_player(&players[0], asteroid)) {
				collision_player(&players[0], asteroid);
				add_dirty_object(players[0].statusObj);
				add_dirty_object(asteroid->asteroidObj);
			}
		}
	}
	
	add_dirty_object(players[0].shipObj);
}

void button_handler() {
	if (laserCnt < laserMax) {
		for (int i=0; i<laserMax; i++) {
			laser_elem* laser = &lasers[i];
			if (laser->laserObj->enable == 0) {
				laserCnt++;
				laser->laserObj->enable = 1;
				laser->laserObj->localSpriteIdx = players[0].shipObj->localSpriteIdx;
				laser->laserObj->xPos = players[0].shipObj->xPos;
				laser->laserObj->yPos = players[0].shipObj->yPos;
				laser->laserObj->xFlip = players[0].shipObj->xFlip;
				laser->laserObj->yFlip = players[0].shipObj->yFlip;
				add_dirty_object(laser->laserObj);
				break;
			}
		}
	}
}

bool check_collision_player(player_elem* player, asteroid_elem* asteroid) {
	int playerLeft = player->shipObj->xPos;
	int playerRight = playerLeft + 15;
	int playerUp = player->shipObj->yPos;
	int playerDown = playerUp + 15;

	int asteroidLeft = asteroid->asteroidObj->xPos;
	int asteroidRight = asteroidLeft + 15;
	int asteroidUp = asteroid->asteroidObj->yPos;
	int asteroidDown = asteroidUp + 15;

	return playerLeft <= asteroidRight && playerRight >= asteroidLeft
			 && playerUp <= asteroidDown && playerDown >= asteroidUp;
}

bool check_collision_laser(laser_elem* laser, asteroid_elem* asteroid) {
	int laserLeft = laser->laserObj->xPos + 7;
	int laserRight = laserLeft + 1;
	int laserUp = laser->laserObj->yPos + 7;
	int laserDown = laserUp + 1;

	int asteroidLeft = asteroid->asteroidObj->xPos;
	int asteroidRight = asteroidLeft + 15;
	int asteroidUp = asteroid->asteroidObj->yPos;
	int asteroidDown = asteroidUp + 15;
	return laserLeft <= asteroidRight && laserRight >= asteroidLeft
			 && laserUp <= asteroidDown && laserDown >= asteroidUp;
}

void collision_player(player_elem* player, asteroid_elem* asteroid) {
	if (player->hp == 0) {
		game = 0;
	} else {
		player->hp--;
		player->statusObj->localSpriteIdx++;
		asteroid->asteroidObj->enable = 0;
	}
}

void collision_laser(laser_elem* laser, asteroid_elem* asteroid) {
	laser->laserObj->enable = 0;
	asteroid->isHit = 1;
	asteroid->asteroidObj->localSpriteIdx++;
	laserCnt--;
}

void end_game() {
	delete_objects();
	free(players);
	free(asteroids);
	free(lasers);
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
		print_object(asteroids[i].asteroidObj);
	}
	printf("\n");

	printf("lasers\n");
	for (int i=0; i<laserMax; i++) {
		print_object(lasers[i].laserObj);
	}
	printf("\n\n");
}
