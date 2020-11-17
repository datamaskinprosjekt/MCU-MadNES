#include "game_logic.h"


/**
 * @brief The main game loop
 * 
 * Runs the entire game in a loop
 */
void game_loop()
{
	// Seed randomizer
	srand(ticks);

	// "Semaphore", when true: proceed, when false: wait
	sem_game = true;
	force_quit = false;

	// Set ticks
	lastTicks = ticks;

	// Initialize game
	init_game();

	// Run game loop
	while (!force_quit) {
		// Wait for sem_game to be released
		while(!sem_game);
		sem_game = false;

		// Call game logic frame
		run_logic();
	}

	// Free objects and elements
	clear_all();
}


/**
 * @brief Run the logic for one time frame
 * 
 * Is called every time frame
 * Checks the state and controller input
 * Calculate deltaTicks
 * Calls actions based on controller input
 * Calls time handler
 */
void run_logic()
{
	// Poll controllers
	poll_controllers();

	// Calculate ticks
	deltaTicks = ticks - lastTicks;
	lastTicks = ticks;
	
	// Initialize buttons
	int joyDir;
	int fuelButton;
	int shootButton;
	int restartButton;

	// Handle switching state from GAME OVER to start screen
	if (game_state == GAME_OVER && gameOverTicksCnt > GAME_OVER_DURATION )
	{ 
		gameOverTicksCnt = 0;
		start_screen();
	}

	// Handle GAME OVER state
	if (game_state == GAME_OVER)
	{
		gameOverTicksCnt += deltaTicks;
		game_over_loop();
		return;
	} 

	// Handle START SCREEN state
	if (game_state == GAME_START_SCREEN)
	{
		start_screen_loop();
	}

	// Check controller for each ship
	for (int i=0; i<shipNum; i++) {
		Controller* controller = players[i].controller;

		if (controller != NULL) {
			// Retrieve button values
			joyDir = controller->joyDir;
			fuelButton = controller->btn1;
			shootButton = controller->btn2 || controller->joyBtn;
			restartButton = controller->btn1 || controller->btn2 || controller->joyBtn || (controller->joyDir != -1); 

			// Call action based on buttons
			if (joyDir != -1) {
				joystick_handler(i, joyDir);
			}
			if (fuelButton) {
				button_fuel_handler(i);
			}
			if (shootButton) {
				button_shoot_handler(i);
			}
			if (restartButton) {
				button_start_handler();
			}
		}
	}
	// Run time handler frame
	time_handler();
}


/**
 * @brief Initialize game logic and game elements
 * 
 * Set all delays values
 * Call to initialize all objects
 * Initialize player, status, asteroid, laser, letters and score elements
 */
void init_game()
{
	// Set game state
	game_state = GAME_START_SCREEN;

	// Set number of players, asteroids per player, lasers per player
	playerNum = get_num_active_controllers();
	asteroidPerPlayer = 3;
	laserPerPlayer = 3;

	// Initialize objects
	init_objects(playerNum, asteroidPerPlayer, laserPerPlayer);


	/*
	 * Set number of ticks delay before action
	 */

	// Number of ticks before moving each pixel
	playerPixelTicks = 80;
	// Asteroid speed will increase, so the start value must be saved
	asteroidStartPixelTicks = 400;
	asteroidPixelTicks = asteroidStartPixelTicks;
	laserPixelTicks = 60;

	// Number of ticks before shooting
	shootTicks = 2500;
	// Number of ticks before flickering
	flickerTicks = 5000;
	// Number of ticks before shifting to the next sprite index of an exploding asteroid
	explodeTicks = 1000;

	// Number of ticks before increasing asteroid speed
	increaseAsteroidSpeedTicks = 3000;
	// Current number of ticks before increasing asteroid speed
	increaseAsteroidSpeedTicksCnt = 0;

	// Number of ticks before GAME OVER display is finished
	gameOverTicksCnt = 0;

	
	// Number of flickers everytime a player is hit
	flickerTotal = 6;

	// Malloc arrays of elements
	players = (PlayerElem *) malloc(sizeof(PlayerElem) * playerNum);
	asteroids = (AsteroidElem *) malloc(sizeof(AsteroidElem) * asteroidNum);
	lasers = (LaserElem *) malloc(sizeof(LaserElem) * laserNum);
	letterGameOver = (LetterGameOverElem *) malloc(sizeof(LetterGameOverElem) * num_letters_game_over);
	letterLogo = (LetterLogoElem *) malloc(sizeof(LetterLogoElem) * num_letters_logo);
	letterPushToStart = (LetterPushToStartElem *) malloc(sizeof(LetterPushToStartElem) * num_letters_push_to_start);
	scores = (ScoreElem *) malloc(sizeof(ScoreElem) * scoreNum);

	// First objects are stars
	int objIdx = starNum;

	// Initialize all lasers
	for (int i=0; i<laserNum; i++) {
		lasers[i] = (LaserElem) {i / laserPerPlayer, 0, &objects[objIdx]};
		objIdx++;
	}

	// Initialize all asteroids
	for (int i=0; i<asteroidNum; i++) {
		asteroids[i] = (AsteroidElem) {0, i / asteroidPerPlayer, 0, 0, 0, &objects[objIdx]};
		objIdx++;
	}

	// Initialize all players
	for (int i=0; i<playerNum; i++) {
		players[i] = (PlayerElem) {i, 3, 0, flickerTotal, laserPerPlayer * i, 0, 0, 0, 0, &objects[objIdx], &objects[playerNum + objIdx], get_next_active_controller()};
		objIdx++;
	}
	objIdx += playerNum;

	// Initialize GAME OVER text
	for (int i=0; i<num_letters_game_over; i++) {
		letterGameOver[i] = (LetterGameOverElem) {&objects[objIdx]};
		objIdx++;
	}

	// Initialize ASTEROIDS text
	for (int i=0; i<num_letters_logo; i++) {
		letterLogo[i] = (LetterLogoElem) {&objects[objIdx]};
		objIdx++;
	}

	// Initialize PRESS TO PLAY text
	for (int i=0; i<num_letters_push_to_start; i++) {
		letterPushToStart[i] = (LetterPushToStartElem) {&objects[objIdx]};
		objIdx++;
	}

	// Initialize SCORE display
	for (int i=0; i<scoreNum; i++) {
		scores[i] = (ScoreElem) {&objects[objIdx]};
		objIdx++;
	}

	/// "Notify" that the objects are initialized
    objects_initialized = true;

	// Add the Game Interrupt Handler
	register_interrupt_callback(game_interrupt_handler);

	__game_ready();

}


/**
 * @brief Run a time frame
 * 
 * Update asteroid, laser, and player elements
 * Check for collisions
 * Flicker player
 * Increase asteroid speed using deltaTicks
 */
void time_handler()
{
	if (game_state == GAME_RUN) {

		// Update each asteroid
		for (int i=0; i<asteroidNum; i++) {
			AsteroidElem* asteroid = &asteroids[i];
			if (asteroid->asteroidObj->enabled) {
				if (asteroid->isHit) {

					// Check ticks and update explosion of asteroid
					int currentExplodeTicks = asteroid->explodeTicksCnt + deltaTicks;
					int explodeNum = currentExplodeTicks / explodeTicks;
					asteroid->explodeTicksCnt = currentExplodeTicks % explodeTicks;
					if (explodeNum) {
						if (asteroid->asteroidObj->localSpriteIdx + 1 >= asteroid->asteroidObj->type->length) {
							// Finish explosion
							asteroid->asteroidObj->enabled = 0;
						} else {
							// Continue explosion
							asteroid->asteroidObj->localSpriteIdx++;
						}
					}
				} else {
					// Check ticks and move asteroid toward the player
					int rot = get_asteroid_rotation(asteroid);
					int currentPixelTicks = asteroid->pixelTicksCnt + deltaTicks;
					int pixelNum = currentPixelTicks / asteroidPixelTicks;
					asteroid->pixelTicksCnt = currentPixelTicks % asteroidPixelTicks;
					if (pixelNum) {
						move_object(asteroid->asteroidObj, rot, pixelNum);
					}

					// Check and update collision with players
					for (int j=0; j<shipNum; j++) {
						PlayerElem* player = &players[j];
						if (check_collision_player(player, asteroid)) {
							collision_player(&players[j], asteroid);
							add_dirty_object(players[j].statusObj);
						}
					}

					// Check and update collision with lasers
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
				// Enable and reset asteroid
				asteroid->isHit = 0;
				asteroid->pixelTicksCnt = 0;
				asteroid->explodeTicksCnt = 0;
				asteroid->asteroidObj->enabled = 1;
				asteroid->asteroidObj->localSpriteIdx = 0;
				set_asteroid_pos(asteroid);
			}
			add_dirty_object(asteroid->asteroidObj);
		}

		// Set priorities so colliding asteroids sprites can overlap
		/*for(int firstAsteroid = 0; firstAsteroid < asteroidNum; asteroidNum++)
		{
			for(int secondAsteroid = firstAsteroid + 1; secondAsteroid < asteroidNum; secondAsteroid++)
			{
				AsteroidElem* firstAsteroidElement = &asteroids[firstAsteroid];
				AsteroidElem* secondAsteroidElement = &asteroids[secondAsteroid];
			}
		}*/

		// Move each laser based on deltaTicks
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

		// Check and flicker all player
		for (int i=0; i<shipNum; i++) {
			flicker_player(&players[i]);
		}

		// Gradually increase asteroid speed based on deltaTicks
		int currentIncreaseAsteroidSpeedTicks = increaseAsteroidSpeedTicksCnt + deltaTicks;
		int increaseAsteroidSpeedNum = currentIncreaseAsteroidSpeedTicks / increaseAsteroidSpeedTicks;
		increaseAsteroidSpeedTicksCnt = currentIncreaseAsteroidSpeedTicks % increaseAsteroidSpeedTicks;
		if (increaseAsteroidSpeedNum && asteroidPixelTicks > 50) {
			asteroidPixelTicks--;
		}
	}
}


/**
 * @brief Rotate player ship
 * 
 * Is called when joystick is moved
 * Sets the sprite index, xFlip and yFlip for the player ship based on joystick rotation
 * 
 * @param playerIdx The player index of the PlayerElem* players array
 * @param rot The rotation from controller input
 */
void joystick_handler(int playerIdx, int rot)
{
	if (game_state == GAME_RUN) {

		// Check if player is alive
		PlayerElem* player = &players[playerIdx];
		if (!player->hp && !player->flickerDownCnt) {
			return;
		}

		// Apply odd rotations [1, 3, 5, 6, 9, 11, 13, 15]
		int rotDiff = (players[playerIdx].controller->joyDir - players[playerIdx].controller->prevJoyDir) % 16;

		// Set rot between previous and new rotation if the difference is 2
		if (rotDiff == 2)
		{
			rot = (rot - 1) % 16;
		}
		else if (rotDiff == 14)
		{
			rot = (rot + 1) % 16;
		}
		
		// Initialization
		int dir = rot / 4;
		int spriteIdx = rot % 4;
		int xFlip = 0;
		int yFlip = 0;

		// Check rotation quadrant
		switch(dir) {
			case 1:
				// Set rotation on second quadrant
				spriteIdx = 8 - rot;
				xFlip = rot == 4? 0 : 1;
				break;
			case 2:
				// Set rotation on third quadrant
				spriteIdx = rot - 8;
				xFlip = 1;
				yFlip = rot == 8? 0 : 1;
				break;
			case 3:
				// Set rotation on fourth quadrant
				spriteIdx = 16 - rot;
				yFlip = 1;
				break;
		}

		// Set ship rotation by updating sprite index, xFlip and yFlip
		player->shipObj->localSpriteIdx = spriteIdx;
		player->shipObj->xFlip = yFlip;
		player->shipObj->yFlip = xFlip;

		add_dirty_object(player->shipObj);
	}
}


/**
 * @brief Move the player
 * 
 * Is called when fuel button is pushed
 * Moves player in its current rotation with speed based on deltaTicks
 * 
 * @param playerIdx The player index of the PlayerElem* players array
 */
void button_fuel_handler(int playerIdx)
{
	if (game_state == GAME_RUN) {

		// Check if player is alive
		PlayerElem* player = &players[playerIdx];
		if (!player->hp && !player->flickerDownCnt) {
			return;
		}

		// Find the speed based on deltaTicks
		int currentPixelTicks = player->pixelTicksCnt + deltaTicks;
		int pixelNum = currentPixelTicks / playerPixelTicks;
		player->pixelTicksCnt = currentPixelTicks % playerPixelTicks;
		if (pixelNum) {

			// Move player with the calculated speed and current rotation
			move_object(player->shipObj, get_rot(player->shipObj), pixelNum);
			add_dirty_object(player->shipObj);
		}
	}
}


/**
 * @brief Run shoot from player
 * 
 * Is called when shoot button is pushed
 * Enable the next laser from player input if the shoot delay is finished
 * The laser will get the same position and direction as the player
 * 
 * @param playerIdx The player index of the PlayerElem* players array
 */
void button_shoot_handler(int playerIdx)
{
	if (game_state == GAME_RUN) {

		// Check if player is alive
		PlayerElem* player = &players[playerIdx];
		if (!player->hp || !player->flickerDownCnt) {
			return;
		}

		// Check if the shoot delay is finished by using deltaTicks
		int currentShootTicks = player->shootTicksCnt + deltaTicks;
		int shootNum = currentShootTicks / shootTicks;
		player->shootTicksCnt = currentShootTicks % shootTicks;
		if (shootNum) {

			// Enable and position the next laser
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


/**
 * @brief Run restart
 * 
 * Is called when a player pushes restart button while the game is in START SCREEN state
 * Must hide START and GAME OVER text, and reset game elements
 */
void button_start_handler()
{

	if (game_state != GAME_RUN) {
		game_state = GAME_RUN;

		// Hide GAME OVER text
		for (int i=0; i<num_letters_game_over; i++) {
			LetterGameOverElem* letter = &letterGameOver[i];
			letter->letterGameOverObj->enabled = 0;
			add_dirty_object(letter->letterGameOverObj);
		}

		// Hide ASTEROIDS text
		for (int i=0; i<num_letters_logo; i++) {
			LetterLogoElem* letter = &letterLogo[i];
			letter->letterLogoObj->enabled = 0;
			add_dirty_object(letter->letterLogoObj);
		}

		// Hide PRESS TO PLAY text
		for (int i=0; i<num_letters_push_to_start; i++) {
			LetterPushToStartElem* letter = &letterPushToStart[i];
			letter->letterPushToStartObj->enabled = 0;
			add_dirty_object(letter->letterPushToStartObj);
		}

		// Reset player attributes
		for (int i=0; i<shipNum; i++) {
			PlayerElem* player = &players[i];
			player->hp = 3;
			player->isHit = 0;
			player->flickerDownCnt = flickerTotal;
			player->localLaserNext = 0;
			player->pixelTicksCnt = 0;
			player->shootTicksCnt = 0;
			player->flickerTicksCnt = 0;
			player->shipObj->enabled = 1;
			player->shipObj->localSpriteIdx = 0;
			player->shipObj->xPos = 150 + 50*i;
			player->shipObj->yPos = 400;
			player->shipObj->xFlip = 0;
			player->shipObj->yFlip = 0;
			player->statusObj->enabled = 1;
			player->statusObj->localSpriteIdx = 0;
			add_dirty_object(player->shipObj);
			add_dirty_object(player->statusObj);
		}

		// Reset asteroid attributes
		for (int i=0; i<asteroidNum; i++) {
			AsteroidElem* asteroid = &asteroids[i];
			set_asteroid_pos(asteroid);
			asteroid->isHit = 0;
			asteroid->playerIdx = i / asteroidPerPlayer;
			asteroid->pixelTicksCnt = 0;
			asteroid->explodeTicksCnt = 0;
			asteroid->prevCollideState = 0;
			asteroid->asteroidObj->enabled = 1;
			asteroid->asteroidObj->localSpriteIdx = 0;
			add_dirty_object(asteroid->asteroidObj);
		}

		// Reset laser attributes
		for (int i=0; i<laserNum; i++) {
			LaserElem* laser = &lasers[i];
			laser->pixelTicksCnt = 0;
			laser->laserObj->enabled = 0;
			add_dirty_object(lasers->laserObj);
		}

		// Reset SCORE display
		for (int i=0; i<scoreNum; i++) {
			ScoreElem* score = &scores[i];
			score->scoreObj->enabled = 1;
			score->scoreObj->localSpriteIdx = 0;
			add_dirty_object(score->scoreObj);
		}

		// Reset asteroid speed
		asteroidPixelTicks = asteroidStartPixelTicks;
		increaseAsteroidSpeedTicksCnt = 0;
	}
}


/**
 * @brief Set asteroid positions
 * 
 * Set random X and Y position for asteroid
 * It must be at least a hundred pixels away from each player
 * 
 * @param asteroid The asteroid element to reposition
 */
void set_asteroid_pos(AsteroidElem* asteroid)
{
	// Initialize player and position
	PlayerElem* player;
    int xPos = 0;
    int yPos = 0;

	// Run while until a valid position is found
    bool seek = 1;
    while (seek) {

		// Set random X and Y position
        xPos = rand() % (WIDTH - 16);
        yPos = rand() % (HEIGHT - 16);

		// Check if position is valid
		bool invalid = 0;
		for (int i=0; i<shipNum; i++) {
			player = &players[i];
			int playerXPos = player->shipObj->xPos;
			int playerYPos = player->shipObj->yPos;

			// Check if position is within a hundred pixel of player
			if (player->hp && xPos >= (playerXPos - 100) && xPos <= (playerXPos + 100) && yPos >= (playerYPos - 100) && yPos <= (playerYPos + 100)) {
				invalid = 1;
			}
		}

		// Check if new position is valid
		if (!invalid) {
			seek = 0;
		}
    }

	// Set new position
    asteroid->asteroidObj->xPos = xPos;
    asteroid->asteroidObj->yPos = yPos;
}


/**
 * @brief Set asteroid new player element
 * 
 * When a player dies, the asteroids is set another player
 * It can only be called when it's NOT GAME OVER
 * 
 * @param oldPlayer 
 */
void set_asteroid_new_player(PlayerElem* oldPlayer)
{
	// Initialize next player element
	int oldPlayerIdx = oldPlayer->id;
	int nextPlayerIdx = 0;
	PlayerElem* nextPlayer = &players[nextPlayerIdx];

	// Set new player for all asteroids of old player
	for (int i=0; i<asteroidNum; i++) {
		AsteroidElem* asteroid = &asteroids[i];
		if (asteroid->playerIdx == oldPlayerIdx) {

			// Seek for next alive player to set as new player
			bool seek = 1;
			while (seek) {
				if (nextPlayer->hp || nextPlayer->flickerDownCnt) {
					seek = 0;
				} else {
					nextPlayerIdx = (nextPlayerIdx + 1) % shipNum;
					nextPlayer = &players[nextPlayerIdx];
				}
			}

			// Set next player for asteroid
			asteroid->playerIdx = nextPlayerIdx;
			nextPlayerIdx = (nextPlayerIdx + 1) % shipNum;
			nextPlayer = &players[nextPlayerIdx];
		}
	}
}


/**
 * @brief Get the asteroid rotation
 * 
 * Asteroids are always moving towards its set player
 * Rotation is calculated based on its and its player's location
 * Rotation is an interger [0, 15] going clockwise starting from the upwards direction
 * 
 * @param asteroid The asteroid element
 * @return The current rotation
 */
int get_asteroid_rotation(AsteroidElem* asteroid)
{
	// Get the X and Y distance between asteroid and its player
	PlayerElem* player = &players[asteroid->playerIdx];
	int xDiff = asteroid->asteroidObj->xPos - player->shipObj->xPos;
	int yDiff = asteroid->asteroidObj->yPos - player->shipObj->yPos;

	// Calculate rotation
	if (xDiff != 0 || yDiff != 0) {
		int rot;
		int xDiffAbs = abs(xDiff);
		int yDiffAbs = abs(yDiff);

		// First find the rotation relative to the first quadrant
		if (xDiffAbs < yDiffAbs)
		{
			// Calculate rot, it must be 0, 1, or 2
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
			// Calculate rot, it must be 2, 3, or 4
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
			// Rot must be 2
			rot = 2;
		}

		// Flip the rot to the correct quadrant
		// Asteroid rotation must be the opposite of the player's direction towards the asteroid
		if (xDiff < 0 && yDiff <= 0)
		{
			// Flip rot to second quadrant
			rot = 8 - rot;
		}
		else if (xDiff >= 0 && yDiff <= 0)
		{
			// Flip rot to third quadrant
			rot += 8;
		}
		else if (xDiff > 0 && yDiff >= 0)
		{
			// Flip rot to fourth quadrant
			rot = (16 - rot) % 16;
		}

		return rot;
	}
	return 0;
}


/**
 * @brief Flicker the player
 * 
 * Check and count down flickers and check for GAME OVER
 * Update player depending on HP and number of flickers
 * 
 * @param player The flickering player
 */
void flicker_player(PlayerElem* player)
{
	if (player->isHit) {
		// Check ticks for flickering timer
		int currentFlickerTicks = player->flickerTicksCnt + deltaTicks;
		int flickerNum = currentFlickerTicks / flickerTicks;
		player->flickerTicksCnt = currentFlickerTicks % flickerTicks;
		if (flickerNum) {
			if (player->flickerDownCnt) {
				// Flip enable value of ship
				player->flickerDownCnt--;
				player->shipObj->enabled = !player->shipObj->enabled;
			}
			else {
				// End of flickering
				player->isHit = 0;
				player->flickerTicksCnt = 0;
				if (player->hp) {
					// Continue alive player
					player->flickerDownCnt = flickerTotal;
					player->shipObj->enabled = 1;
				}
				else {
					// Hide dead player and check for GAME OVER
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
 * @return whether overlapping
 **/
bool check_collision_player(PlayerElem* player, AsteroidElem* asteroid)
{
	// Check if player is alive
	if (!player->hp && !player->flickerDownCnt) {
		return;
	}

	// Calculate bounding box coordinates for player
	int playerLeft = player->shipObj->xPos;
	int playerRight = playerLeft + 15;
	int playerUp = player->shipObj->yPos;
	int playerDown = playerUp + 15;

	// Calculate bounding box coordinates for asteroid
	int asteroidLeft = asteroid->asteroidObj->xPos;
	int asteroidRight = asteroidLeft + 15;
	int asteroidUp = asteroid->asteroidObj->yPos;
	int asteroidDown = asteroidUp + 15;

	// Return overlap ship and asteroid
	return playerLeft <= asteroidRight && playerRight >= asteroidLeft
			 && playerUp <= asteroidDown && playerDown >= asteroidUp;
}


/**
 * Checks whether the bounding boxes of two asteroid objects overlap 
 * 
 * @param asteroid1 The first asteroid element to check
 * @param asteroid2 The second asteroid element to check
 * @return whether overlapping
 **/
bool check_collision_asteroid(AsteroidElem* asteroid1, AsteroidElem* asteroid2)
{
	// Calculate bounding box coordinates for asteroid1
	int asteroid1Left = asteroid1->asteroidObj->xPos;
	int asteroid1Right = asteroid1Left + 15;
	int asteroid1Up = asteroid1->asteroidObj->yPos;
	int asteroid1Down = asteroid1Up + 15;

	// Calculate bounding box coordinates for asteroid2
	int asteroid2Left = asteroid2->asteroidObj->xPos;
	int asteroid2Right = asteroid2Left + 15;
	int asteroid2Up = asteroid2->asteroidObj->yPos;
	int asteroid2Down = asteroid2Up + 15;

	// Return overlap asteroid1 and asteroid2
	return asteroid1Left <= asteroid2Right && asteroid1Right >= asteroid2Left
			 && asteroid1Up <= asteroid2Down && asteroid1Down >= asteroid2Up;
}


/**
 * Checks whether the bounding boxes of a laser and an asteroid overlap 
 * 
 * @param laser The laser element to check
 * @param asteroid The asteroid element to check
 * @return whether overlapping
 */
bool check_collision_laser(LaserElem* laser, AsteroidElem* asteroid)
{
	// Calculate bounding box coordinates for laser
	int laserLeft = laser->laserObj->xPos + 7;
	int laserRight = laserLeft + 1;
	int laserUp = laser->laserObj->yPos + 7;
	int laserDown = laserUp + 1;

	// Calculate bounding box coordinates for asteroid
	int asteroidLeft = asteroid->asteroidObj->xPos;
	int asteroidRight = asteroidLeft + 15;
	int asteroidUp = asteroid->asteroidObj->yPos;
	int asteroidDown = asteroidUp + 15;

	// Return overlap laser and asteroid
	return laserLeft <= asteroidRight && laserRight >= asteroidLeft
			 && laserUp <= asteroidDown && laserDown >= asteroidUp;
}


/**
 * Run collision between a player and an asteroid
 * 
 * @param player The player element to check
 * @param asteroid The asteroid element to check
 */
void collision_player(PlayerElem* player, AsteroidElem* asteroid)
{
	// Update player hp and status and asteroid
	if (!player->isHit) {
		player->hp--;
		player->isHit = 1;
		player->statusObj->localSpriteIdx++;
	}
	asteroid->isHit = 1;
}


/**
 * Run collision between a laser and an asteroid
 * 
 * @param laser The laser element to check
 * @param asteroid The asteroid element to check
 */
void collision_laser(LaserElem* laser, AsteroidElem* asteroid)
{
	// Update laser and asteroid
	laser->laserObj->enabled = 0;
	asteroid->isHit = 1;
	
	// Get score data for the player
	PlayerElem* player = &players[laser->playerIdx];
	ScoreElem* scoreLeft = &scores[player->id * 2];
	ScoreElem* scoreRight = &scores[player->id * 2 + 1];

	// Update score for the player
	scoreRight->scoreObj->localSpriteIdx = (scoreRight->scoreObj->localSpriteIdx + 1) % 10;
	if (!scoreRight->scoreObj->localSpriteIdx) {
		scoreLeft->scoreObj->localSpriteIdx = (scoreLeft->scoreObj->localSpriteIdx + 1) % 10;
	}
	add_dirty_object(scoreRight->scoreObj);
	add_dirty_object(scoreLeft->scoreObj);
}


/**
 * Checks if it is GAME OVER by checking active ships
 * @return if GAME OVER
 */
bool check_game_over()
{
	// Check if there are any alive ships
	for (int i=0; i<shipNum; i++) {
		if (players[i].hp || players[i].flickerDownCnt) {
			return 0;
		}
	}
	return 1;
}


/**
 * @brief Runs during the Game Over display
 * 
 * Explodes asteroids, move active lasers, and flicker Game Over display
 */
void game_over_loop()
{

	gameOverFlickerTicksCnt += deltaTicks;

	// Explode asteroids
	for(int i = 0; i < asteroidNum; i++)
	{
		AsteroidElem* asteroid = &asteroids[i];

		int currentExplodeTicks = asteroid->explodeTicksCnt + deltaTicks;
		int explodeNum = currentExplodeTicks / explodeTicks;
		asteroid->explodeTicksCnt = currentExplodeTicks % explodeTicks;
		if (explodeNum) {
			if (asteroid->asteroidObj->localSpriteIdx + 1 >= asteroid->asteroidObj->type->length) {
				asteroid->asteroidObj->enabled = 0;
			} else {
				asteroid->asteroidObj->localSpriteIdx++;
			}
			
			add_dirty_object(asteroid->asteroidObj);
		}
	}

	// Move active lasers
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

	// Flicker the Game Over display
	if (gameOverTicksCnt > 1000) {
		if (gameOverFlickerTicksCnt > 7500) {
			gameOverFlickerTicksCnt = 0;
			for(int i = 0; i < num_letters_game_over; i++)
			{
				LetterGameOverElem* letter = &letterGameOver[i];

				letter->letterGameOverObj->enabled = !letter->letterGameOverObj->enabled;

				add_dirty_object(letter->letterGameOverObj);
			}
		}
	}
	
}


/**
 * @brief Set up the Game Over state
 * 
 * Hide ships and statusbars and show Game Over display
 */
void game_over()
{
	game_state = GAME_OVER;

	// Reset GAME OVER delay ticks
	gameOverTicksCnt = 0;
	gameOverFlickerTicksCnt = 0;

	// Hide ships and statusbars
	for (int i=0; i<playerNum; i++) {
		PlayerElem* player = &players[i];
		player->shipObj->enabled = 0;
		player->statusObj->enabled = 0;
		add_dirty_object(player->statusObj);
	}

	// Display the Game Over text
	for (int i=0; i<num_letters_game_over; i++) {
		LetterGameOverElem* letter = &letterGameOver[i];
		letter->letterGameOverObj->enabled = 1;
		add_dirty_object(letter->letterGameOverObj);
	}
}


/**
 * @brief Runs during the Start Screen display
 * 
 * Flicker the PRESS TO PLAY text
 */
void start_screen_loop()
{
	pressToStartFlickerCounter += deltaTicks;

	// Flicker PRESS TO PLAY text
	if(pressToStartFlickerCounter > 7500) {
		pressToStartFlickerCounter = 0;
		for (int i = 0; i < num_letters_push_to_start; i++) {
			LetterPushToStartElem* letter = &letterPushToStart[i];
			letter->letterPushToStartObj->enabled = !letter->letterPushToStartObj->enabled;
			add_dirty_object(letter->letterPushToStartObj);
		}
	}
}


/**
 * @brief Set up Start Screen
 * 
 * Display the ASTEROIDS and PUSH TO PLAY text
 * Hide the GAME OVER text and the SCORE
 */
void start_screen()
{
	game_state = GAME_START_SCREEN;

	pressToStartFlickerCounter = 0;

	// Hide GAME OVER text
	for(int i = 0; i < num_letters_game_over; i++)
	{
		LetterGameOverElem* letter = &letterGameOver[i];
		letter->letterGameOverObj->enabled = false;
		add_dirty_object(letter->letterGameOverObj);
	}

	// Show the ASTEROIDS text
	for(int i = 0; i < num_letters_logo; i++) {
		LetterLogoElem* letter = &letterLogo[i];
		letter->letterLogoObj->enabled = true;
		add_dirty_object(letter->letterLogoObj);
	}

	// Show the PUSH TO PLAY text
	for(int i = 0; i < num_letters_push_to_start; i++) {
		LetterPushToStartElem* letter = &letterPushToStart[i];
		letter->letterPushToStartObj->enabled = true;
		add_dirty_object(letter->letterPushToStartObj);
	}

	// Hide the SCORE
	for (int i = 0; i < scoreNum; i++) {
		ScoreElem* score = &scores[i];
		score->scoreObj->enabled = false;
		add_dirty_object(score->scoreObj);
	}
}


/**
 * Free memory of all Elements and Objects
 */
void clear_all()
{
	// Free memory of all objects
	delete_objects();

	// Free memory of all elements
	free(players);
	free(asteroids);
	free(lasers);
	free(letterGameOver);
	free(letterLogo);
	free(letterPushToStart);
}


/**
 * @brief User-space interrupt handler
 * 
 * This is the game-specific interrupt handler. It is registered with register_interrupt_callback()
 * 
 * @see init_game()
 * @see register_interrupt_callback()
 */
void game_interrupt_handler()
{
	// Remove?
	//if (!objects_initialized) { return; }
    
	// Unlock the game semaphore
    sem_game = true;
}
