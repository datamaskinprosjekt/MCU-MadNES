#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "object.h"
#include "game_logic.h"

int main(void) {
	// TODO seed with actual time
	srand(0);
	//srand(time(NULL));
	// search for roundNo and remove all
	roundNo = 3;
	// move the if (game) to the interrupt calls to avoid unnecessary repeats
	game = 1;

	init_game();
	test_print();
	button_shoot_handler(1);

	int tmp = roundNo + 1;
	// should be while(1), but is limited due to testing
	while (roundNo) {
		if (roundNo < tmp) {
			button_restart_handler();
			tmp--;
			printf("\nGAME ROUND %d\n\n", roundNo);
		}
		if (roundNo == 3) {
			test_game_print();
		}
		time_handler(100);
	}
	test_game_print();
	test_print();

	clear_all();

	return 0;
}

// "Semaphore"
// When true, proceed
// When false, wait
bool sem_game = true;
uint32_t currentTicks = 0;

void main_logic() {
	init_game();

	while (1) {

		// Wait for sem_game to be released
		while(sem_game);
		// get controller inputs
		run_logic();
	}
}

void run_logic() {
	// Time since last time run_logic was called
	static uint32_t lastTickCount;
	static uint32_t deltaTicks;

	// Ticks should be predefined by the SysClk module
	deltaTicks = currentTicks - lastTickCount;
	// check buttons and joystick
	for (int i=0; i<shipMax; i++) {
		int fuelButton = 0;
		int shootButton = 0;
		int restartButton = 0;
		if (fuelButton) {
			button_fuel_handler(deltaTicks, i);
		}
		if (shootButton) {
			button_shoot_handler(i);
		}
		if (restartButton) {
			button_restart_handler(i);
		}
	}
	time_handler(deltaTicks);
}

void init_game() {
	init_objects();

	playerPixelTicks = 50;
	asteroidPixelTicks = 50;
	laserPixelTicks = 20;
	flickerTicks = 200;
	explodeTicks = 100;
	
	asteroidCnt = asteroidMax / shipMax;
	flickerCnt = 6;
	laserCnt = laserMax / shipMax;

	players = (player_elem *) malloc(sizeof(player_elem) * shipMax);
	asteroids = (asteroid_elem *) malloc(sizeof(asteroid_elem) * asteroidMax);
	lasers = (laser_elem *) malloc(sizeof(laser_elem) * laserMax);
	letters = (letter_elem *) malloc(sizeof(letter_elem) * letterMax);

	int objIdx = 0;
	for (int i=0; i<shipMax; i++) {
		players[i] = (player_elem) {i, 3, 0, flickerCnt, laserCnt * i, 0, 0, 0, &objects[i], &objects[shipMax + i]};
		objIdx += 2;
	}
	for (int i=0; i<asteroidMax; i++) {
		asteroids[i] = (asteroid_elem) {0, i / asteroidCnt, 0, 0, &objects[objIdx]};
		set_asteroid_pos(&asteroids[i]);
		add_dirty_object(asteroids[i].asteroidObj);
		objIdx++;
	}
	for (int i=0; i<laserMax; i++) {
		lasers[i] = (laser_elem) {i / laserCnt, 0, &objects[objIdx]};
		objIdx++;
	}
	for (int i=0; i<letterMax; i++) {
		letters[i] = (letter_elem) {&objects[objIdx]};
		objIdx++;
	}
}

void time_handler(int ticks) {
	if (game) {
		for (int i=0; i<asteroidMax; i++) {
			asteroid_elem* asteroid = &asteroids[i];
			if (asteroid->asteroidObj->enabled) {
				if (asteroid->isHit) {
					int currentExplodeTicks = asteroid->explodeTicksCnt + ticks;
					int explodeNum = currentExplodeTicks / explodeTicks;
					asteroid->explodeTicksCnt = currentExplodeTicks % explodeTicks;
					if (explodeNum) {
						if (asteroid->asteroidObj->localSpriteIdx + 1 >= asteroid->asteroidObj->type->length) {
							asteroid->asteroidObj->enabled = 0;
						} else {
							asteroid->asteroidObj->localSpriteIdx++;
						}
					}
				} else {
					int rot = get_asteroid_rotation(asteroid);
					int currentPixelTicks = asteroid->pixelTicksCnt + ticks;
					int pixelNum = currentPixelTicks / asteroidPixelTicks;
					asteroid->pixelTicksCnt = currentPixelTicks % asteroidPixelTicks;
					if (pixelNum) {
						move_object(asteroid->asteroidObj, rot, pixelNum);
					}

					for (int j=0; j<shipMax; j++) {
						if (check_collision_player(&players[j], asteroid)) {
							collision_player(&players[j], asteroid);
							add_dirty_object(players[j].statusObj);
						}
					}

					for (int j=0; j<laserMax; j++) {
						laser_elem* laser = &lasers[j];
						if (laser->laserObj->enabled) {
							if (check_collision_laser(laser, asteroid)) {
								collision_laser(laser, asteroid);
								add_dirty_object(laser->laserObj);
							}
						}
					}
				}
			} else {
				asteroid->isHit = 0;
				asteroid->pixelTicksCnt = 0;
				asteroid->explodeTicksCnt = 0;
				asteroid->asteroidObj->enabled = 1;
				asteroid->asteroidObj->localSpriteIdx = 0;
				set_asteroid_pos(asteroid);
			}
			add_dirty_object(asteroid->asteroidObj);
		}
		for (int i=0; i<laserMax; i++) {
			laser_elem* laser = &lasers[i];
			if (laser->laserObj->enabled) {
				int laserRot = get_rot(laser->laserObj);
				int currentPixelTicks = laser->pixelTicksCnt + ticks;
				int pixelNum = currentPixelTicks / laserPixelTicks;
				laser->pixelTicksCnt = currentPixelTicks % laserPixelTicks;
				if (pixelNum) {
					int moved = move_object(laser->laserObj, laserRot, pixelNum);
					if (!moved) {
						laser->laserObj->enabled = 0;
						players[laser->playerIdx].laserActiveCnt--;
					}
					add_dirty_object(laser->laserObj);
				}
			}
		}
		for (int i=0; i<shipMax; i++) {
			flicker_player(ticks, &players[i]);
		}
	}
}

void joystick_handler(int playerIdx, int rot) {
	if (game) {
		if (!players[playerIdx].hp) {
			return;
		}
		Object* shipObj = players[playerIdx].shipObj;

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

void button_fuel_handler(int ticks, int playerIdx) {
	if (game) {
		if (!players[playerIdx].hp) {
			return;
		}
		player_elem* player = &players[playerIdx];
		int currentPixelTicks = player->pixelTicksCnt + ticks;
		int pixelNum = currentPixelTicks / playerPixelTicks;
		player->pixelTicksCnt = currentPixelTicks % playerPixelTicks;
		if (pixelNum) {
			move_object(player->shipObj, get_rot(player->shipObj), pixelNum);
			add_dirty_object(player->shipObj);
		}
	}
}

void button_shoot_handler(int playerIdx) {
	if (game) {
		player_elem* player = &players[playerIdx];
		if (!player->hp) {
			return;
		}
		if (player->laserActiveCnt < laserCnt) {
			for (int i=player->laserIdx; i<player->laserIdx+laserCnt; i++) {
				laser_elem* laser = &lasers[i];
				if (laser->laserObj->enabled == 0) {
					player->laserActiveCnt++;
					laser->pixelTicksCnt = 0;
					laser->laserObj->enabled = 1;
					laser->laserObj->localSpriteIdx = player->shipObj->localSpriteIdx;
					laser->laserObj->xPos = player->shipObj->xPos;
					laser->laserObj->yPos = player->shipObj->yPos;
					laser->laserObj->xFlip = player->shipObj->xFlip;
					laser->laserObj->yFlip = player->shipObj->yFlip;
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
		for (int i=0; i<shipMax; i++) {
			player_elem* player = &players[i];
			player->hp = 3;
			player->isHit = 0;
			player->flickerDownCnt = flickerCnt;
			player->laserActiveCnt = 0;
			player->pixelTicksCnt = 0;
			player->flickerTicksCnt = 0;
			player->shipObj->enabled = 1;
			player->shipObj->localSpriteIdx = 0;
			player->shipObj->xPos = 150 + 50*i;
			player->shipObj->yPos = 400;
			player->shipObj->xFlip = 0;
			player->shipObj->yFlip = 0;
			player->statusObj->localSpriteIdx = 0;
			add_dirty_object(player->shipObj);
			add_dirty_object(player->statusObj);
		}
		for (int i=0; i<asteroidMax; i++) {
			asteroid_elem* asteroid = &asteroids[i];
			set_asteroid_pos(asteroid);
			asteroid->isHit = 0;
			asteroid->playerIdx = i / asteroidCnt;
			asteroid->pixelTicksCnt = 0;
			asteroid->explodeTicksCnt = 0;
			asteroid->asteroidObj->enabled = 1;
			asteroid->asteroidObj->localSpriteIdx = 0;
			add_dirty_object(asteroid->asteroidObj);
		}
		for (int i=0; i<laserMax; i++) {
			laser_elem* laser = &lasers[i];
			laser->pixelTicksCnt = 0;
			laser->laserObj->enabled = 0;
			add_dirty_object(lasers->laserObj);
		}
	}
}

void set_asteroid_pos(asteroid_elem* asteroid) {
    int xPos = 0;
    int yPos = 0;

    bool seek = 1;
    while (seek) {
        xPos = rand() % (WIDTH - 16);
        yPos = rand() % (HEIGHT - 16);
		bool invalid = 0;
		for (int i=0; i<shipMax; i++) {
			player_elem* player = &players[i];
			int playerXPos = player->shipObj->xPos;
			int playerYPos = player->shipObj->yPos;
			if (player->hp && xPos >= (playerXPos - 100) && xPos <= (playerXPos + 100) && yPos >= (playerYPos - 100) && yPos <= (playerYPos + 100)) {
				invalid = 1;
			}
		}
		if (!invalid) {
			seek = 0;
		}
    }
    asteroid->asteroidObj->xPos = xPos;
    asteroid->asteroidObj->yPos = yPos;
}

void set_asteroid_new_player(player_elem* oldPlayer) {
	int oldPlayerIdx = oldPlayer->id;
	int nextPlayerIdx = 0;
	player_elem* nextPlayer = &players[nextPlayerIdx];

	for (int i=0; i<asteroidMax; i++) {
		asteroid_elem* asteroid = &asteroids[i];
		if (asteroid->playerIdx == oldPlayerIdx) {
			bool seek = 1;
			while (seek) {
				if (nextPlayer->hp || nextPlayer->flickerDownCnt) {
					seek = 0;
				} else {
					nextPlayerIdx = (nextPlayerIdx + 1) % shipMax;
					nextPlayer = &players[nextPlayerIdx];
				}
			}
			asteroid->playerIdx = nextPlayerIdx;
			nextPlayerIdx = (nextPlayerIdx + 1) % shipMax;
			nextPlayer = &players[nextPlayerIdx];
		}
	}
}

int get_asteroid_rotation(asteroid_elem* asteroid) {
	player_elem* player = &players[asteroid->playerIdx];
	int xDiff = asteroid->asteroidObj->xPos - player->shipObj->xPos;
	int yDiff = asteroid->asteroidObj->yPos - player->shipObj->yPos;
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
		else if (yDiffAbs < xDiffAbs)
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

		return rot;
	}
	return 0;
}

void flicker_player(int ticks, player_elem* player) {
	if (player->isHit) {
		int currentFlickerTicks = player->flickerTicksCnt + ticks;
		int flickerNum = currentFlickerTicks / flickerTicks;
		player->flickerTicksCnt = currentFlickerTicks % flickerTicks;
		if (flickerNum) {
			if (player->flickerDownCnt) {
				player->flickerDownCnt--;
				player->shipObj->enabled = !player->shipObj->enabled;
			}
			else {
				player->isHit = 0;
				player->flickerTicksCnt = 0;
				if (player->hp) {
					player->flickerDownCnt = flickerCnt;
					player->shipObj->enabled = 1;
				}
				else {
					player->shipObj->enabled = 0;
					if (check_game_over()) {
						game_over();
					} else {
						set_asteroid_new_player(player);
					}
				}
			}
			add_dirty_object(player->shipObj);
		}
	}
}

bool check_collision_player(player_elem* player, asteroid_elem* asteroid) {
	if (!player->hp || player->isHit) {
		return 0;
	}
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
	player->hp--;
	player->isHit = 1;
	player->statusObj->localSpriteIdx++;
	asteroid->isHit = 1;
}

void collision_laser(laser_elem* laser, asteroid_elem* asteroid) {
	laser->laserObj->enabled = 0;
	asteroid->isHit = 1;
	players[laser->playerIdx].laserActiveCnt--;
}

bool check_game_over() {
	for (int i=0; i<shipMax; i++) {
		if (players[i].hp || players[i].flickerDownCnt) {
			return 0;
		}
	}
	return 1;
}

void game_over() {
	roundNo --;
	game = 0;
	for (int i=0; i<letterMax; i++) {
		letter_elem* letter = &letters[i];
		letter->letterObj->enabled = 1;
		add_dirty_object(letter->letterObj);
	}
}

void clear_all() {
	delete_objects();
	free(players);
	free(asteroids);
	free(lasers);
	free(letters);
}

void test_game_print() {
	for (int i=0; i<shipMax; i++) {
		Object* shipObj = players[i].shipObj;
		Object* statusObj = players[i].statusObj;
		printf("player - statusIdx %d - hp %d - pos (%d, %d) - isHit %d - flickerDownCnt %d - laserIdx %d - laserCnt %d - enable %d\n",
		statusObj->localSpriteIdx, players[i].hp, shipObj->xPos, shipObj->yPos, players[i].isHit, players[i].flickerDownCnt, players[i].laserIdx, players[i].laserActiveCnt, shipObj->enabled);
	}
	for (int i=0; i<asteroidMax; i++) {
		Object* asteroidObj = asteroids[i].asteroidObj;
		printf("asteroid %d - idx %d - hit %d - pos(%d, %d) - playerIdx %d - enable %d\n",
		asteroidObj->id, asteroidObj->localSpriteIdx, asteroids[i].isHit, asteroidObj->xPos, asteroidObj->yPos, asteroids[i].playerIdx, asteroidObj->enabled);
	}
	for (int i=0; i<laserMax; i++) {
		Object* laserObj = lasers[i].laserObj;
		printf("laser %d - pos (%d, %d) - playerIdx %d - enable %d\n",
		laserObj->id, laserObj->xPos, laserObj->yPos, lasers[i].playerIdx, laserObj->enabled);
	}
	printf("\n");
}

void test_print() {
	for (int i=0; i<shipMax; i++) {
		printf("player %d\n", players[i].shipObj->id);
		print_object(players[i].shipObj);
		print_object(players[i].statusObj);
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
