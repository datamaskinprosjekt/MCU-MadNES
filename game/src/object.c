#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "object.h"


/**
 * @brief Initialize entity types
 * 
 * Initialize sprite type of SHIP, STATUS, ASTEROID, LASER, LETTER_GAME_OVER, LETTER_LOGO, LETTER_PRESS_TO_START, SCORE, STAR
 * 
 * @see struct SpriteType
 */
void init_entity_types()
{    
    //Initialize ship types
    shipTypes[0] = (SpriteType) {SHIP, 0, 5};       // ship1Type
    shipTypes[1] = (SpriteType) {SHIP, 5, 5};       // ship2Type
    shipTypes[2] = (SpriteType) {SHIP, 10, 5};      // ship3Type
    shipTypes[3] = (SpriteType) {SHIP, 15, 5};      // ship4Type
    shipTypes[4] = (SpriteType) {SHIP, 20, 5};      // ship5Type
    shipTypes[5] = (SpriteType) {SHIP, 25, 5};      // ship6Type
    shipTypes[6] = (SpriteType) {SHIP, 30, 5};      // ship7Type
    shipTypes[7] = (SpriteType) {SHIP, 35, 5};      // ship8Type
    shipTypes[8] = (SpriteType) {SHIP, 40, 5};      // ship9Type

    // Initialize status types
    statusTypes[0] = (SpriteType)  {STATUS, 45, 4};
    statusTypes[1] = (SpriteType)  {STATUS, 49, 4};
    statusTypes[2] = (SpriteType)  {STATUS, 53, 4};
    statusTypes[3] = (SpriteType)  {STATUS, 57, 4};
    statusTypes[4] = (SpriteType)  {STATUS, 61, 4};
    statusTypes[5] = (SpriteType)  {STATUS, 65, 4};
    statusTypes[6] = (SpriteType)  {STATUS, 69, 4};
    statusTypes[7] = (SpriteType)  {STATUS, 73, 4};
    statusTypes[8] = (SpriteType)  {STATUS, 77, 4};

    // Initialize asteroid types
    asteroidTypes[0] = (SpriteType) {ASTEROID, 81, 11};
    asteroidTypes[1] = (SpriteType) {ASTEROID, 92, 11};
    asteroidTypes[2] = (SpriteType) {ASTEROID, 103, 16};

    // Initialize laser type
    laserType = (SpriteType) {LASER, 119, 5};

    // Initialize letter game over type
    letterGameOverType = (SpriteType) {LETTER_GAME_OVER, 124, 8};

    // Initialize letter logo type
    letterLogoType = (SpriteType) {LETTER_LOGO, 132, 20};

    // Initialize number type
    scoreType = (SpriteType) {SCORE, 152, 10};

    // Initialize star type
    starType = (SpriteType) {STAR, 162, 6};
    
    // Set number for each type of star
    starTypeNumber[0] = 7;
    starTypeNumber[1] = 5;
    starTypeNumber[2] = 6;
    starTypeNumber[3] = 5;
    starTypeNumber[4] = 3;
    starTypeNumber[5] = 4;
}


/**
 * @brief Initialize all objects
 * 
 * Set number of each type of object: ship, asteroid, laser, stars, score, GAME OVER text, logo, PRESS TO PLAY text
 * Initialize each object with distinct ids
 * Calculate and set necessary positions of objects
 * 
 * @param playerNum The number of players
 * @param asteroidPerPlayer The number of asteroids per player
 * @param laserPerPlayer The number of lasers per player
 */
void init_objects(int playerNum, int asteroidPerPlayer, int laserPerPlayer)
{
    // Call to initialize entity types
    init_entity_types();

    // Set star positions
    int star1Pos[2 * 7] = {62,136, 227,380, 283,174, 321,278, 408,88, 488,252, 582,174};
    int star2Pos[2 * 5] = {122,112, 158,273, 392,113, 469,380, 527,262};
    int star3Pos[2 * 6] = {85,331, 124,53, 329,93, 416,305, 477,128, 530,411};
    int star4Pos[2 * 5] = {86,419, 107,205, 252,64, 408,224, 547,40};
    int star5Pos[2 * 3] = {45,103, 230,146, 309,417};
    int star6Pos[2 * 4] = {258,209, 355,235, 489,63, 535,311};

    int* starPositions[NUM_STAR_TYPES] = {
        star1Pos,
        star2Pos,
        star3Pos,
        star4Pos,
        star5Pos,
        star6Pos
    };

    // Set number of each type of object
    objNum = 0;
    shipNum = playerNum;
    statusNum = shipNum;
    asteroidNum = asteroidPerPlayer * MAX_CONTROLLERS;
    laserNum = laserPerPlayer * MAX_CONTROLLERS;
    num_letters_game_over = 8;
    num_letters_logo = 9;
    num_letters_push_to_start = 11;
    // Initialize enough scores for 8 players
    scoreNum = MAX_CONTROLLERS * 2;
    starNum = 30;

    // Malloc objects and dirty_objects
    objects = (Object *) malloc(sizeof(Object) * MAX_OBJS);
    dirty_objects = (int *) malloc(sizeof(int) * MAX_OBJS);
    
    memset(objects, 0, sizeof(Object) * MAX_OBJS);
    memset(dirty_objects, 0, sizeof(int) * MAX_OBJS);


    // Initialize star objects
    for (int i = 0; i < NUM_STAR_TYPES; i++) {
        for (int j = 0; j < starTypeNumber[i]; j++) {                               
            objects[objNum] = (Object) {objNum, &starType, i, starPositions[i][2 * j], starPositions[i][2 * j + 1], 0, 0, 0, 1};
            add_dirty_object(&objects[objNum]);
            objNum++;
        }
    }

    // Initialize laser objects
    for (int i=0; i<laserNum; i++) {
        objects[objNum] = (Object) {objNum, &laserType, 0, 0, 0, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize asteroid objects
    for (int i=0; i<asteroidNum; i++) {
        objects[objNum] = (Object) {objNum, &asteroidTypes[i % NUM_ASTEROID_TYPES], 0, 0, 0, 0, 0, 0, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize ship objects
    for (int i=0; i<shipNum; i++) {
        objects[objNum] = (Object) {objNum, &shipTypes[i % NUM_SHIP_TYPES], 0, 150 + 50 * i, 400, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize ship objects
    for (int i = shipNum; i < MAX_CONTROLLERS; i++) {
        objects[objNum] = (Object) {objNum, &shipTypes[i % NUM_SHIP_TYPES], 0, 150 + 50 * i, 400, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize active status objects, enabled
    for (int i=0; i<statusNum; i++) {
        if (i==0) {
            statusObjIdx = objNum;
        }
        objects[objNum] = (Object) {objNum, &statusTypes[i % NUM_STATUS_TYPES], 0, WIDTH - 16*(statusNum * 2 - i * 2), 0, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize remaining status objects, disabled
    for (int i=statusNum; i < MAX_CONTROLLERS; i++) {
        objects[objNum] = (Object) {objNum, &statusTypes[i % NUM_STATUS_TYPES], 0, WIDTH - 16*(statusNum * 2 - i * 2), 0, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize GAME OVER text objects and calculate positions
    int xGameOverBase = WIDTH / 2 - 32;
    int yGameOverBase = HEIGHT / 2 - 16;
    for (int i=0; i<num_letters_game_over; i++) {
        int xGameOver = xGameOverBase + 16 * (i % 4);
        int yGameOver = yGameOverBase + 16 * (i / 4);
        objects[objNum] = (Object) {objNum, &letterGameOverType, i, xGameOver, yGameOver, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize ASTEROIDS text objects and calculate positions
    int xLogoBase = WIDTH / 2 - 16 * 4 - 8;
    int yLogoBase = HEIGHT / 2 - 32;
    for (int i=0; i<num_letters_logo; i++) {
        int xLogo = xLogoBase + 16 * i;
        objects[objNum] = (Object) {objNum, &letterLogoType, i, xLogo, yLogoBase, 0, 0, 1, 1};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize PRESS TO PLAY text objects and calculate positions
    xLogoBase = WIDTH / 2 - 16 * 6 - 8;
    yLogoBase = HEIGHT / 2 + 16;

    int spaces_encountered = 0;

    for (int i=0; i<num_letters_push_to_start; i++) {

        int xLogo = xLogoBase + 16 * i + 16 * spaces_encountered;
        if ( (i == 5) | (i == 7) ) { spaces_encountered++; }

        xLogo += (i == 5 ? 16 : 0);
        xLogo += (i == 7 ? 16 : 0);
        objects[objNum] = (Object) {objNum, &letterLogoType, num_letters_logo + i, xLogo, yLogoBase, 0, 0, 1, 1};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Initialize SCORE objects and calculate positions
    int xScoreBase = 0;
    int yScoreBase = 0;
    for (int i=0; i<scoreNum; i++) {
        xScoreBase += 16;
        xScoreBase += i % 2 == 0 ? 16 : 0;
        objects[objNum] = (Object) {objNum, &scoreType, 0, xScoreBase, yScoreBase, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }
}


/**
 * @brief Set an object as dirty
 * 
 * Set the object index of dirty_objects to true
 * In the next frame, the object will be updated to the screen
 * After each frame, all dirty_objects is set to false
 * 
 * @param obj The object to be set
 */
void add_dirty_object(Object* obj)
{
    dirty_objects[obj->id] = 1;
}


/**
 * @brief Move an object with a specified rotation and speed
 * 
 * Calculate the new X and Y position based on the object's current position, specified rotation and speed
 * 
 * @param obj The Object to be moved
 * @param rot The rotation direction of the movement
 * @param speed The speed of the movement
 * @return Whether the object was moved
 */
int move_object(Object* obj, int rot, int speed)
{
    // The new X and Y positions to be set
    int xNew;
    int yNew;

    // The movement change to be set
	int xOffset = 0;
	int yOffset = 0;

    // Calculate angle and quadrant
	int angle = rot % 4;
	int dir = rot / 4;
	int tmp = -1;

    // Check angle relative to the first quadrant and set offsets
	switch(angle) {
		case 0:
			yOffset -= speed;
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

    // Flip the offsets to the correct quadrant
	switch(dir) {
		case 1:
            // Second quadrant
			tmp = xOffset;
			xOffset = -yOffset;
			yOffset = tmp;
			break;
		case 2:
            // Third quadrant
			xOffset = -xOffset;
			yOffset = -yOffset;
			break;
		case 3:
            // Fourth quadrant
			tmp = xOffset;
			xOffset = yOffset;
			yOffset = -tmp;
			break;
	}

    // Calculate new positions
	xNew = obj->xPos + xOffset;
	yNew = obj->yPos + yOffset;

    // Check if objects touches the borders of the screen
	if (xNew < 0 || xNew + 15 >= WIDTH || yNew < 0 || yNew + 15 >= HEIGHT) {
		return 0;
	}

    // Set new positions
	obj->xPos = xNew;
	obj->yPos = yNew;

    return 1;
}


/**
 * @brief Get the rotation of an object
 * 
 * Calculate a rotation value [0, 15], which is clockwise starting from upwards
 * 
 * @param Object The object to get the rotation from
 * @return The rotation of the object
 */
int get_rot(Object* Object) {

    // xFlip and yFlip must be swapped
    bool xFlip = Object->yFlip;
    bool yFlip = Object->xFlip;

    if (!xFlip && !yFlip)
    {
        // Rotation is in the first quadrant
        return Object->localSpriteIdx;
    }
    else if (xFlip && !yFlip)
    {
        // Rotation is in the second quadrant
        return 8 - Object->localSpriteIdx;
    }
    else if (xFlip && yFlip)
    {
        // Rotation is in the third quadrant
        return Object->localSpriteIdx + 8;
    }
    // Rotation is in the fourth quadrant
    return (16 - Object->localSpriteIdx) % 16;
}


/**
 * Free all objects
 */
void delete_objects() {
    free(objects);
    free(dirty_objects);
}
