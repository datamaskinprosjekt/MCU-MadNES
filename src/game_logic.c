#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "object.h"
#include "game_logic.h"

int main(void) {
	srand(time(NULL));
	// search for roundNo and remove all
	roundNo = 3;
	// move the if(game) to the interrupt calls to avoid unnecessary repeats
	game = 1;

	init_game();
	test_print();
	button_shoot_handler();

	int tmp = roundNo + 1;
	// should be while(1), but is limited due to testing
	while (roundNo) {
		if (roundNo < tmp) {
			button_restart_handler();
			tmp--;
			printf("\nGAME ROUND %d\n\n", roundNo);
		}
		time_handler();
		if (roundNo == 3) {
			test_game_print();
		}
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
	letters = (letter_elem *) malloc(sizeof(letter_elem) * letterMax);

	int cnt = 0;
	for (int i=0; i<shipMax; i++) {
		players[i] = (player_elem) {2, &objs[i], &objs[shipMax + i]};
	}
	cnt += shipMax + statusMax;
	for (int i=0; i<asteroidMax; i++) {
		asteroids[i] = (asteroid_elem) {asteroidSpeed, 0, &objs[cnt + i]};
	}
	cnt += asteroidMax;
	for (int i=0; i<laserMax; i++) {
		lasers[i] = (laser_elem) {&objs[cnt + i]};
	}
	cnt += laserMax;
	for (int i=0; i<letterMax; i++) {
		letters[i] = (letter_elem) {&objs[cnt + i]};
	}
}

void time_handler() {	
	if (game) {
		for (int i=0; i<asteroidMax; i++) {
			asteroid_elem* asteroid = &asteroids[i];
			if (asteroid->asteroidObj->enabled) {
				if (asteroid->isHit) {
					if (asteroid->asteroidObj->localSpriteIdx + 1 >= asteroid->asteroidObj->type->length) {
						asteroid->asteroidObj->enabled = 0;
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
						if (laser->laserObj->enabled) {
							int laserRot = get_rot(laser->laserObj);
							int moved = move_object(laser->laserObj, laserRot, laserSpeed);
							if (!moved) {
								laser->laserObj->enabled = 0;
								laserCnt--;
							}

							if (check_collision_laser(laser, asteroid)) {
								collision_laser(laser, asteroid);
							}

							add_dirty_object(laser->laserObj);
						}
					}
				}
			} else {
				asteroid->isHit = 0;
				asteroid->asteroidObj->enabled = 1;
				asteroid->asteroidObj->localSpriteIdx = 0;
				int xPos = 0;
				int yPos = 0;
				bool seek = 1;
				int playerXPos = players[0].shipObj->xPos;
				int playerYPos = players[0].shipObj->yPos;
				while (seek) {
					xPos = rand() % (WIDTH - 16);
					yPos = rand() % (HEIGHT - 16);
					if (xPos <= (playerXPos - 100) || xPos >= (playerXPos + 100) || yPos <= (playerYPos - 100) || yPos >= (playerYPos + 100)) {
						seek = 0;
					}
				}
				asteroid->asteroidObj->xPos = xPos;
				asteroid->asteroidObj->yPos = yPos;
				asteroidCnt++;
			}
			add_dirty_object(asteroid->asteroidObj);
		}
	}
}

void joystick_handler(int rot) {
	if (game) {
		Object* shipObj = players[0].shipObj;

		int dir = rot / 4;
		int spriteIdx = rot % 4;
		int xFlip = 0;
		int yFlip = 0;

		switch(dir) {
			case 1:
				spriteIdx = 8 - rot;
				xFlip = rot == 4? 0 : 1;
				break;
			case 2:
				spriteIdx = rot - 8;
				xFlip = 1;
				yFlip = rot == 8? 0 : 1;
				break;
			case 3:
				spriteIdx = 16 - rot;
				yFlip = 1;
				break;
		}

		shipObj->localSpriteIdx = spriteIdx;
		shipObj->xFlip = xFlip;
		shipObj->yFlip = yFlip;

		add_dirty_object(shipObj);
	}
}

void button_fuel_handler() {
	if (game) {
		Object* shipObj = players[0].shipObj;
		move_object(shipObj, get_rot(shipObj), playerSpeed);
		add_dirty_object(shipObj);
	}
}

void button_shoot_handler() {
	if (game) {
		if (laserCnt < laserMax) {
			for (int i=0; i<laserMax; i++) {
				laser_elem* laser = &lasers[i];
				if (laser->laserObj->enabled == 0) {
					laserCnt++;
					laser->laserObj->enabled = 1;
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
}

void button_restart_handler() {
	if (!game) {
		game = 1;
		for (int i=0; i<letterMax; i++) {
			letter_elem* letter = &letters[i];
			letter->letterObj->enabled = 0;
			add_dirty_object(letter->letterObj);
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
		roundNo --;
		game = 0;
		for (int i=0; i<letterMax; i++) {
			letter_elem* letter = &letters[i];
			letter->letterObj->enabled = 1;
			add_dirty_object(letter->letterObj);
		}
	} else {
		player->hp--;
	}
	asteroid->asteroidObj->enabled = 0;
	player->statusObj->localSpriteIdx++;
}

void collision_laser(laser_elem* laser, asteroid_elem* asteroid) {
	laser->laserObj->enabled = 0;
	asteroid->isHit = 1;
	asteroid->asteroidObj->localSpriteIdx++;
	laserCnt--;
}

void end_game() {
	delete_objects();
	free(players);
	free(asteroids);
	free(lasers);
	free(letters);
}

void test_game_print() {
	printf("player - statusIdx %d - hp %d - pos (%d, %d)\n", players[0].statusObj->localSpriteIdx, players[0].hp, players[0].shipObj->xPos, players[0].shipObj->yPos);
	for (int i=0; i<asteroidMax; i++) {
		Object* asteroidObj = asteroids[i].asteroidObj;
		printf("asteroid %d - idx %d - hit %d - pos(%d, %d) - enable %d\n", asteroidObj->id, asteroidObj->localSpriteIdx, asteroids[i].isHit, asteroidObj->xPos, asteroidObj->yPos, asteroidObj->enable);
	}
	for (int i=0; i<laserMax; i++) {
		Object* laserObj = lasers[i].laserObj;
		printf("laser %d - pos (%d, %d) - enable %d\n", laserObj->id, laserObj->xPos, laserObj->yPos, laserObj->enable);
	}
	printf("\n");
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
	printf("\n");

	printf("letters\n");
	for (int i=0; i<letterMax; i++) {
		print_object(letters[i].letterObj);
	}
	printf("\n\n");
}
