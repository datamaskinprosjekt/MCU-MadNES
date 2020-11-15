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

void game_loop()
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
	poll_controllers();

	deltaTicks = ticks - lastTicks;
	lastTicks = ticks;
	
	int joyDir;
	int prevJoyDir;
	int fuelButton;
	int shootButton;
	int restartButton;

	// Handle switching state from GAME_OVER to start screen
	if(game_state == GAME_OVER) gameOverCounter += deltaTicks;
	if(game_state == GAME_OVER && gameOverCounter > GAME_OVER_DURATION ) { 
		gameOverCounter = 0;
		game_state = GAME_START_SCREEN;
	}




	for (int i=0; i<shipNum; i++) {
		Controller* controller = players[i].controller;

		if (controller != NULL) {
			joyDir = controller->joyDir;
			prevJoyDir = controller->prevJoyDir;
			fuelButton = controller->btn1;
			shootButton = controller->btn2 || controller->joyBtn;
			restartButton = controller->btn1 || controller->btn2 || controller->joyBtn || controller->joyDir != -1; //?

			if (joyDir != -1) {
				joystick_handler(i, joyDir, prevJoyDir);
			}
			if (fuelButton) {
				button_fuel_handler(deltaTicks, i);
			}
			if (shootButton) {
				button_shoot_handler(deltaTicks, i);
			}
			if (restartButton) {
				button_start_handler();
			}
		}
	}
	time_handler(deltaTicks);
}

void test_main()
{
	srand(time(NULL));

	init_game();

	test_print();
	button_shoot_handler(100, 1);
	
	int roundNo = 3;
	while (roundNo) {



		if (game_state != GAME_RUN) {
			button_start_handler();
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
	game_state = GAME_START_SCREEN;

	gameOverCounter = 0;

	playerNum = get_num_active_controllers();
	asteroidPerPlayer = 2;
	laserPerPlayer = 3;
	init_objects(playerNum, asteroidPerPlayer, laserPerPlayer);

	playerPixelTicks = 300;
	asteroidPixelTicks = 500;
	laserPixelTicks = 70;
	shootTicks = 2500;
	rotDelayTicks = 100;
	flickerTicks = 5000;
	explodeTicks = 1000;
	
	flickerTotal = 6;

	players = (PlayerElem *) malloc(sizeof(PlayerElem) * playerNum);
	asteroids = (AsteroidElem *) malloc(sizeof(AsteroidElem) * asteroidNum);
	lasers = (LaserElem *) malloc(sizeof(LaserElem) * laserNum);
	letterGameOver = (LetterGameOverElem *) malloc(sizeof(LetterGameOverElem) * num_letters_game_over);
	letterLogo = (LetterLogoElem *) malloc(sizeof(LetterLogoElem) * num_letters_logo);
	letterPushToStart = (LetterPushToStartElem *) malloc(sizeof(LetterPushToStartElem) * num_letters_push_to_start);

	int objIdx = starNum;
	for (int i=0; i<laserNum; i++) {
		lasers[i] = (LaserElem) {i / laserPerPlayer, 0, &objects[objIdx]};
		objIdx++;
	}
	for (int i=0; i<asteroidNum; i++) {
		asteroids[i] = (AsteroidElem) {0, i / asteroidPerPlayer, 0, 0, &objects[objIdx]};
		objIdx++;
	}
	for (int i=0; i<playerNum; i++) {
		players[i] = (PlayerElem) {i, 3, 0, flickerTotal, laserPerPlayer * i, 0, 0, 0, 0, 0, &objects[objIdx], &objects[playerNum + objIdx], get_next_active_controller()};
		objIdx++;
	}
	objIdx += playerNum;
	for (int i=0; i<num_letters_game_over; i++) {
		letterGameOver[i] = (LetterGameOverElem) {&objects[objIdx]};
		objIdx++;
	}
	for (int i=0; i<num_letters_logo; i++) {
		letterLogo[i] = (LetterLogoElem) {&objects[objIdx]};
		objIdx++;
	}
	for (int i=0; i<num_letters_push_to_start; i++) {
		letterPushToStart[i] = (LetterPushToStartElem) {&objects[objIdx]};
		objIdx++;
	}

	for (int i=0; i<asteroidNum; i++) {
		set_asteroid_pos(&asteroids[i]);
		add_dirty_object(asteroids[i].asteroidObj);
	}

	/// "Notify" that the objects are initialized
    objects_initialized = true;

}

void time_handler(int deltaTicks)
{
	if (game_state == GAME_RUN) {
		for (int i=0; i<asteroidNum; i++) {
			AsteroidElem* asteroid = &asteroids[i];
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

					for (int j=0; j<shipNum; j++) {
						PlayerElem* player = &players[j];
						if (check_collision_player(player, asteroid)) {
							collision_player(&players[j], asteroid);
							add_dirty_object(players[j].statusObj);
						}
					}

					for (int j=0; j<laserNum; j++) {
						LaserElem* laser = &lasers[j];
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
		for (int i=0; i<laserNum; i++) {
			LaserElem* laser = &lasers[i];
			if (laser->laserObj->enabled) {
				int laserRot = get_rot(laser->laserObj);
				int currentPixelTicks = laser->pixelTicksCnt + deltaTicks;
				int pixelNum = currentPixelTicks / laserPixelTicks;
				laser->pixelTicksCnt = currentPixelTicks % laserPixelTicks;
				if (pixelNum) {
					int moved = move_object(laser->laserObj, laserRot, pixelNum);
					if (!moved) {
						laser->laserObj->enabled = 0;
					}
					add_dirty_object(laser->laserObj);
				}
			}
		}
		for (int i=0; i<shipNum; i++) {
			flicker_player(deltaTicks, &players[i]);
		}
	}
}

void joystick_handler(int playerIdx, int rot, int prevRot)
{
	if (game_state == GAME_RUN) {
		PlayerElem* player = &players[playerIdx];
		if (!player->hp) {
			return;
		}

		if (rot == (prevRot - 2) % 16) {
			rot = (prevRot + 1) % 16;
		} else if (rot == (prevRot + 2) % 16) {
			rot = (prevRot - 1) % 16;
		}

		if (prevRot % 2) {
			int currentRotDelayTicks = player->rotDelayTicksCnt + deltaTicks;
			int rotDelayNum = currentRotDelayTicks / rotDelayTicks;
			player->rotDelayTicksCnt = currentRotDelayTicks % rotDelayTicks;

			if (!rotDelayNum) {
				return;
			}
		}

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

		player->shipObj->localSpriteIdx = spriteIdx;
		player->shipObj->xFlip = yFlip;
		player->shipObj->yFlip = xFlip;

		add_dirty_object(player->shipObj);
	}
}

void button_fuel_handler(int deltaTicks, int playerIdx)
{
	if (game_state == GAME_RUN) {
		if (!players[playerIdx].hp) {
			return;
		}
		PlayerElem* player = &players[playerIdx];
		int currentPixelTicks = player->pixelTicksCnt + deltaTicks;
		int pixelNum = currentPixelTicks / playerPixelTicks;
		player->pixelTicksCnt = currentPixelTicks % playerPixelTicks;
		if (pixelNum) {
			move_object(player->shipObj, get_rot(player->shipObj), pixelNum);
			add_dirty_object(player->shipObj);
		}
	}
}

void button_shoot_handler(int deltaTicks, int playerIdx)
{
	if (game_state == GAME_RUN) {
		PlayerElem* player = &players[playerIdx];
		if (!player->hp) {
			return;
		}

		int currentShootTicks = player->shootTicksCnt + deltaTicks;
		int shootNum = currentShootTicks / shootTicks;
		player->shootTicksCnt = currentShootTicks % shootTicks;

		if (shootNum) {
			LaserElem* laser = &lasers[player->laserIdx + player->localLaserNext];
			player->localLaserNext = (player->localLaserNext + 1) % laserPerPlayer;
			laser->pixelTicksCnt = 0;
			laser->laserObj->enabled = 1;
			laser->laserObj->localSpriteIdx = player->shipObj->localSpriteIdx;
			laser->laserObj->xPos = player->shipObj->xPos;
			laser->laserObj->yPos = player->shipObj->yPos;
			laser->laserObj->xFlip = player->shipObj->xFlip;
			laser->laserObj->yFlip = player->shipObj->yFlip;
			add_dirty_object(laser->laserObj);
		}
	}
}

void button_start_handler()
{
	if (game_state != GAME_RUN) {
		game_state = GAME_RUN;
		// Disable all letters
		for (int i=0; i<num_letters_game_over; i++) {
			LetterGameOverElem* letterGameOver = &letterGameOver[i];
			letterGameOver->letterGameOverObj->enabled = 0;
			add_dirty_object(letterGameOver->letterGameOverObj);
		}
		for (int i=0; i<num_letters_logo; i++) {
			LetterLogoElem* letterLogo = &letterLogo[i];
			letterLogo->letterLogoObj->enabled = 0;
			add_dirty_object(letterLogo->letterLogoObj);
		}
		for (int i=0; i<num_letters_push_to_start; i++) {
			LetterPushToStartElem* letterPushToStart = &letterPushToStart[i];
			letterPushToStart->letterPushToStartObj->enabled = 0;
			add_dirty_object(letterPushToStart->letterPushToStartObj);
		}

		for (int i=0; i<shipNum; i++) {
			PlayerElem* player = &players[i];
			player->hp = 3;
			player->isHit = 0;
			player->flickerDownCnt = flickerTotal;
			player->localLaserNext = 0;
			player->pixelTicksCnt = 0;
			player->shootTicksCnt = 0;
			player->rotDelayTicksCnt = 0;
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
		for (int i=0; i<asteroidNum; i++) {
			AsteroidElem* asteroid = &asteroids[i];
			set_asteroid_pos(asteroid);
			asteroid->isHit = 0;
			asteroid->playerIdx = i / asteroidPerPlayer;
			asteroid->pixelTicksCnt = 0;
			asteroid->explodeTicksCnt = 0;
			asteroid->asteroidObj->enabled = 1;
			asteroid->asteroidObj->localSpriteIdx = 0;
			add_dirty_object(asteroid->asteroidObj);
		}
		for (int i=0; i<laserNum; i++) {
			LaserElem* laser = &lasers[i];
			laser->pixelTicksCnt = 0;
			laser->laserObj->enabled = 0;
			add_dirty_object(lasers->laserObj);
		}
	}
}

void set_asteroid_pos(AsteroidElem* asteroid)
{
	PlayerElem* player;
	
    int xPos = 0;
    int yPos = 0;

    bool seek = 1;
    while (seek) {
        xPos = rand() % (WIDTH - 16);
        yPos = rand() % (HEIGHT - 16);
		bool invalid = 0;
		for (int i=0; i<shipNum; i++) {
			player = &players[i];
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

void set_asteroid_new_player(PlayerElem* oldPlayer)
{
	int oldPlayerIdx = oldPlayer->id;
	int nextPlayerIdx = 0;
	PlayerElem* nextPlayer = &players[nextPlayerIdx];

	for (int i=0; i<asteroidNum; i++) {
		AsteroidElem* asteroid = &asteroids[i];
		if (asteroid->playerIdx == oldPlayerIdx) {
			bool seek = 1;
			while (seek) {
				if (nextPlayer->hp || nextPlayer->flickerDownCnt) {
					seek = 0;
				} else {
					nextPlayerIdx = (nextPlayerIdx + 1) % shipNum;
					nextPlayer = &players[nextPlayerIdx];
				}
			}
			asteroid->playerIdx = nextPlayerIdx;
			nextPlayerIdx = (nextPlayerIdx + 1) % shipNum;
			nextPlayer = &players[nextPlayerIdx];
		}
	}
}

int get_asteroid_rotation(AsteroidElem* asteroid)
{
	PlayerElem* player = &players[asteroid->playerIdx];
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

void flicker_player(int deltaTicks, PlayerElem* player)
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
bool check_collision_player(PlayerElem* player, AsteroidElem* asteroid)
{
	if (!player->hp && !player->flickerDownCnt) {
		return;
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
bool check_collision_laser(LaserElem* laser, AsteroidElem* asteroid)
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

void collision_player(PlayerElem* player, AsteroidElem* asteroid)
{
	if (!player->isHit) {
		player->hp--;
		player->isHit = 1;
		player->statusObj->localSpriteIdx++;
	}
	asteroid->isHit = 1;
}

void collision_laser(LaserElem* laser, AsteroidElem* asteroid)
{
	laser->laserObj->enabled = 0;
	asteroid->isHit = 1;
}

bool check_game_over()
{
	for (int i=0; i<shipNum; i++) {
		if (players[i].hp || players[i].flickerDownCnt) {
			return 0;
		}
	}
	return 1;
}

void game_over()
{
	game_state = GAME_OVER;
	gameOverCounter = 0;
	for (int i=0; i<num_letters_game_over; i++) {
		LetterGameOverElem* letterGameOver = &letterGameOver[i];
		letterGameOver->letterGameOverObj->enabled = 1;
		add_dirty_object(letterGameOver->letterGameOverObj);
	}
}

void start_screen()
{
	game_state = GAME_START_SCREEN;
	for(int i = 0; i < num_letters_logo; i++) {
		LetterLogoElem* letterLogo = &letterLogo[i];
		letterLogo->letterLogoObj->enabled = true;
		add_dirty_object(letterLogo->letterLogoObj);
	}

	for(int i = 0; i < num_letters_push_to_start; i++) {
		LetterPushToStartElem* letterPushToStart = &letterPushToStart[i];
		letterPushToStart->letterPushToStartObj->enabled = true;
		add_dirty_object(letterPushToStart->letterPushToStartObj);
	}
}

void clear_all()
{
	delete_objects();
	free(players);
	free(asteroids);
	free(lasers);
	free(letterGameOver);
	free(letterLogo);
	free(letterPushToStart);
}

void test_game_print()
{
	for (int i=0; i<shipNum; i++) {
		Object* shipObj = players[i].shipObj;
		Object* statusObj = players[i].statusObj;
		printf("player - statusIdx %d - hp %d - pos (%d, %d) - isHit %d - flickerDownCnt %d - laserIdx %d - laserCnt %d - enable %d\n",
		statusObj->localSpriteIdx, players[i].hp, shipObj->xPos, shipObj->yPos, players[i].isHit, players[i].flickerDownCnt, players[i].laserIdx, players[i].localLaserNext, shipObj->enabled);
	}
	for (int i=0; i<asteroidNum; i++) {
		Object* asteroidObj = asteroids[i].asteroidObj;
		printf("asteroid %d - idx %d - hit %d - pos(%d, %d) - playerIdx %d - enable %d\n",
		asteroidObj->id, asteroidObj->localSpriteIdx, asteroids[i].isHit, asteroidObj->xPos, asteroidObj->yPos, asteroids[i].playerIdx, asteroidObj->enabled);
	}
	for (int i=0; i<laserNum; i++) {
		Object* laserObj = lasers[i].laserObj;
		printf("laser %d - pos (%d, %d) - playerIdx %d - enable %d\n",
		laserObj->id, laserObj->xPos, laserObj->yPos, lasers[i].playerIdx, laserObj->enabled);
	}
	printf("\n");
}

void test_print()
{
	for (int i=0; i<shipNum; i++) {
		printf("player %d\n", players[i].shipObj->id);
		print_object(players[i].shipObj);
		print_object(players[i].statusObj);
		printf("\n");
	}

	printf("asteroids\n");
	for (int i=0; i<asteroidNum; i++) {
		print_object(asteroids[i].asteroidObj);
	}
	printf("\n");

	printf("lasers\n");
	for (int i=0; i<laserNum; i++) {
		print_object(lasers[i].laserObj);
	}
	printf("\n");

	printf("letters\n");
	for (int i=0; i<num_letters_game_over; i++) {
		print_object(letterGameOver[i].letterGameOverObj);
	}
	printf("\n\n");
}
