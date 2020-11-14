#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "object.h"
#include "controllers.h"
#include "meta_data.h"
#include "sys_time.h"
#include "game_logic.h"

/*int main(void) {
	// test_main();
	main_logic();

	return 0;
} */

void main_logic()
{
	srand(ticks);

	// "Semaphore", when true: proceed, when false: wait
	sem_game = true;
	force_quit = false;

	// TODO set last ticks
	lastTicks = ticks;

	init_game();

	//test_print();

	while (!force_quit) {
		// Wait for sem_game to be released
		while(!sem_game) {
			//simulate_semaphore();
		}
		sem_game = false;

		test_game_print();

		run_logic();
	}

	test_print();

	clear_all();
}

void run_logic()
{
	deltaTicks = ticks - lastTicks;
	lastTicks = ticks;
	
	int joyDir;
	int fuelButton;
	int shootButton;
	int restartButton;

	for (int i=0; i<shipMax; i++) {
		Controller* controller = players[i].controller;

		joyDir = controller->joyDir;
		fuelButton = controller->btn1;
		shootButton = controller->btn2;
		restartButton = controller->joyBtn;

		if (joyDir != -1) {
			joystick_handler(i, joyDir);
		}
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

//int tmpCnt = 5;
//int newTicks = 0;
//void simulate_semaphore()
//{
//	lastTicks = newTicks;
//	newTicks += 100;
//
//	CONTROLLER_INPUTS[1].btn1 = 1;
//
//	sem_game = 1;
//
//
//	if (!tmpCnt) {
//		force_quit = 1;
//	}
//	tmpCnt--;
//}

void test_main()
{
	srand(time(NULL));

	init_game();

	test_print();
	button_shoot_handler(1);
	
	int roundNo = 3;
	while (roundNo) {
		if (!game) {
			button_restart_handler();
			roundNo--;
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
}

void init_game()
{
	game = 1;

	int playerNum = get_num_active_controllers();
	asteroidPerPlayer = 2;
	laserPerPlayer = 3;
	init_objects(playerNum, asteroidPerPlayer, laserPerPlayer);

	playerPixelTicks = 50;
	asteroidPixelTicks = 50;
	laserPixelTicks = 20;
	flickerTicks = 200;
	explodeTicks = 100;
	
	flickerTotal = 6;

	players = (player_elem *) malloc(sizeof(player_elem) * playerNum);
	asteroids = (asteroid_elem *) malloc(sizeof(asteroid_elem) * asteroidMax);
	lasers = (laser_elem *) malloc(sizeof(laser_elem) * laserMax);
	letters = (letter_elem *) malloc(sizeof(letter_elem) * letterMax);

	int objIdx = 0;
	for (int i=0; i<shipMax; i++) {
		players[i] = (player_elem) {i, 3, 0, flickerTotal, laserPerPlayer * i, 0, 0, 0, &objects[i], &objects[shipMax + i], get_next_active_controller()};
		objIdx += 2;
	}
	for (int i=0; i<asteroidMax; i++) {
		asteroids[i] = (asteroid_elem) {0, i / asteroidPerPlayer, 0, 0, &objects[objIdx]};
		set_asteroid_pos(&asteroids[i]);
		add_dirty_object(asteroids[i].asteroidObj);
		objIdx++;
	}
	for (int i=0; i<laserMax; i++) {
		lasers[i] = (laser_elem) {i / laserPerPlayer, 0, &objects[objIdx]};
		objIdx++;
	}
	for (int i=0; i<letterMax; i++) {
		letters[i] = (letter_elem) {&objects[objIdx]};
		objIdx++;
	}
}

void time_handler(int deltaTicks)
{
	if (game) {
		for (int i=0; i<asteroidMax; i++) {
			asteroid_elem* asteroid = &asteroids[i];
			if (asteroid->asteroidObj->enabled) {
				if (asteroid->isHit) {
					int currentExplodeTicks = asteroid->explodeTicksCnt + deltaTicks;
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
					int currentPixelTicks = asteroid->pixelTicksCnt + deltaTicks;
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
				int currentPixelTicks = laser->pixelTicksCnt + deltaTicks;
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
			flicker_player(deltaTicks, &players[i]);
		}
	}
}

void joystick_handler(int playerIdx, int rot)
{
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

void button_fuel_handler(int deltaTicks, int playerIdx)
{
	if (game) {
		if (!players[playerIdx].hp) {
			return;
		}
		player_elem* player = &players[playerIdx];
		int currentPixelTicks = player->pixelTicksCnt + deltaTicks;
		int pixelNum = currentPixelTicks / playerPixelTicks;
		player->pixelTicksCnt = currentPixelTicks % playerPixelTicks;
		if (pixelNum) {
			move_object(player->shipObj, get_rot(player->shipObj), pixelNum);
			add_dirty_object(player->shipObj);
		}
	}
}

void button_shoot_handler(int playerIdx)
{
	if (game) {
		player_elem* player = &players[playerIdx];
		if (!player->hp) {
			return;
		}
		if (player->laserActiveCnt < laserPerPlayer) {
			for (int i=player->laserIdx; i<player->laserIdx+laserPerPlayer; i++) {
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

void button_restart_handler()
{
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
			player->flickerDownCnt = flickerTotal;
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
			asteroid->playerIdx = i / asteroidPerPlayer;
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

void set_asteroid_pos(asteroid_elem* asteroid)
{
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

void set_asteroid_new_player(player_elem* oldPlayer)
{
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

int get_asteroid_rotation(asteroid_elem* asteroid)
{
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

void flicker_player(int deltaTicks, player_elem* player)
{
	if (player->isHit) {
		int currentFlickerTicks = player->flickerTicksCnt + deltaTicks;
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
					player->flickerDownCnt = flickerTotal;
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

/**
 * Checks whether the bounding boxes of a player and an asteroid object overlap 
 * 
 * @param player The player element to check
 * @param asteroid The asteroid element to check
 **/
bool check_collision_player(player_elem* player, asteroid_elem* asteroid)
{
	if (!player->hp || player->isHit) {
		return 0;
	}
	int playerLeft = player->shipObj->xPos;
	int playerRight = playerLeft + 15;
	int playerUp = player->shipObj->yPos;
	int playerDown = playerUp + 15;

	// Calculate bounding box coordinates for asteroid
	int asteroidLeft = asteroid->asteroidObj->xPos;
	int asteroidRight = asteroidLeft + 15;
	int asteroidUp = asteroid->asteroidObj->yPos;
	int asteroidDown = asteroidUp + 15;

	// Return overlap status
	return playerLeft <= asteroidRight && playerRight >= asteroidLeft
			 && playerUp <= asteroidDown && playerDown >= asteroidUp;
}

/**
 * Checks whether the bounding boxes of a laser and an asteroid overlap 
 * 
 * @param laser The laser to check
 * @param asteroid The asteroid element to check
 */
bool check_collision_laser(laser_elem* laser, asteroid_elem* asteroid)
{
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

void collision_player(player_elem* player, asteroid_elem* asteroid)
{
	player->hp--;
	player->isHit = 1;
	player->statusObj->localSpriteIdx++;
	asteroid->isHit = 1;
}

void collision_laser(laser_elem* laser, asteroid_elem* asteroid)
{
	laser->laserObj->enabled = 0;
	asteroid->isHit = 1;
	players[laser->playerIdx].laserActiveCnt--;
}

bool check_game_over()
{
	for (int i=0; i<shipMax; i++) {
		if (players[i].hp || players[i].flickerDownCnt) {
			return 0;
		}
	}
	return 1;
}

void game_over()
{
	game = 0;
	for (int i=0; i<letterMax; i++) {
		letter_elem* letter = &letters[i];
		letter->letterObj->enabled = 1;
		add_dirty_object(letter->letterObj);
	}
}

void clear_all()
{
	delete_objects();
	free(players);
	free(asteroids);
	free(lasers);
	free(letters);
}

void test_game_print()
{
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

void test_print()
{
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
