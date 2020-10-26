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
		sleep(1);
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
				int xDiff = asteroid->asteroidObj->xPos - players[0].shipObj->xPos;
				int yDiff = asteroid->asteroidObj->yPos - players[0].shipObj->yPos;
				if (xDiff != 0 && yDiff != 0) {
					int rot;
					int xDiffAbs = abs(xDiff);
					int yDiffAbs = abs(yDiff);
					if (xDiffAbs < yDiffAbs)
					{
						float tmp = xDiffAbs / yDiffAbs;
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
						float tmp = yDiffAbs / xDiffAbs;
						if (tmp < 0.25) {
							rot = 2;
						} else if (tmp > 0.75) {
							rot = 0;
						} else {
							rot = 1;
						}
					}
					else
					{
						rot = 2;
					}

					if (xDiff >= 0 && yDiff >= 0)
					{
						rot = 8 - rot;
					}
					else if (xDiff <= 0 && yDiff >= 0)
					{
						rot += 8;
					}
					else if (xDiff <= 0 && yDiff <= 0)
					{
						rot += (16 - rot) % 16;
					}

					move_object(asteroid->asteroidObj, rot, asteroid->speed);
				}

				if (check_collision_player(&players[0], asteroid)) {
					collision_player(&players[0], asteroid);
					add_dirty_object(players[0].statusObj);
				}

				for (int j=0; j<laserMax; j++) {
					object* laserObj = &laserObjs[j];
					if (laserObj->enable) {
						int laserRot = get_rot(laserObj);
						move_object(laserObj, laserRot, laserSpeed);

						if (check_collision_laser(laserObj, asteroid)) {
							collision_laser(laserObj, asteroid);
						}

						add_dirty_object(laserObj);
					}
				}
			}
		} else {
			asteroid->isHit = 0;
			asteroid->asteroidObj->enable = 1;
			asteroid->asteroidObj->localSpriteIdx = asteroid->asteroidObj->type->globalSpriteIdx;
			asteroid->asteroidObj->xPos = 0;
			asteroid->asteroidObj->yPos = 0;
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
			object* laserObj = &laserObjs[i];
			if (laserObj->enable == 0) {
				laserCnt++;
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

void move_object(object* obj, int rot, int speed) {
	int xOffset = 0;
	int yOffset = 0;

	int angle = rot % 4;
	int dir = rot / 4;
	int tmp = -1;

	switch(angle) {
		case 0:
			yOffset += speed;
			break;
		case 1:
			xOffset += (speed + 1) / 2;
			yOffset -= speed;
			break;
		case 2:
			xOffset += speed;
			yOffset -= speed;
			break;
		case 3:
			xOffset += speed;
			yOffset -= (speed + 1) / 2;
			break;
	}

	switch(dir) {
		case 1:
			tmp = xOffset;
			xOffset = yOffset;
			yOffset = -tmp;
			break;
		case 2:
			xOffset = -xOffset;
			yOffset = -yOffset;
			break;
		case 3:
			tmp = xOffset;
			xOffset = yOffset;
			yOffset = -tmp;
			break;
	}

	int xNew = obj->xPos + xOffset;
	int yNew = obj->yPos + yOffset;
	if (xNew < 0 || xNew + 15 >= WIDTH || yNew < 0 || yNew + 15 >= HEIGHT) {
		return;
	}

	obj->xPos = xNew;
	obj->yPos = yNew;
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

bool check_collision_laser(object* laserObj, asteroid_elem* asteroid) {
	int laserLeft = laserObj->xPos + 7;
	int laserRight = laserLeft + 1;
	int laserUp = laserObj->yPos + 7;
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
