#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "object.h"

void init_entity_types()
{    
    shipTypes[0] = (SpriteType) {SHIP, 0, 5};       // ship1Type
    shipTypes[1] = (SpriteType) {SHIP, 5, 5};       // ship2Type
    shipTypes[2] = (SpriteType) {SHIP, 10, 5};      // ship3Type
    shipTypes[3] = (SpriteType) {SHIP, 15, 5};      // ship4Type
    shipTypes[4] = (SpriteType) {SHIP, 20, 5};      // ship5Type
    shipTypes[5] = (SpriteType) {SHIP, 25, 5};      // ship6Type
    shipTypes[6] = (SpriteType) {SHIP, 30, 5};      // ship7Type
    shipTypes[7] = (SpriteType) {SHIP, 35, 5};      // ship8Type
    shipTypes[8] = (SpriteType) {SHIP, 40, 5};       // ship9Type

    statusTypes[0] = (SpriteType)  {STATUS, 45, 4};
    statusTypes[1] = (SpriteType)  {STATUS, 49, 4};
    statusTypes[2] = (SpriteType)  {STATUS, 53, 4};
    statusTypes[3] = (SpriteType)  {STATUS, 57, 4};
    statusTypes[4] = (SpriteType)  {STATUS, 61, 4};
    statusTypes[5] = (SpriteType)  {STATUS, 65, 4};
    statusTypes[6] = (SpriteType)  {STATUS, 69, 4};
    statusTypes[7] = (SpriteType)  {STATUS, 73, 4};
    statusTypes[8] = (SpriteType)  {STATUS, 77, 4};

    asteroidTypes[0] = (SpriteType) {ASTEROID, 81, 11};
    asteroidTypes[1] = (SpriteType) {ASTEROID, 92, 11};
    asteroidTypes[2] = (SpriteType) {ASTEROID, 103, 16};

    // Initialize laser type
    laserType = (SpriteType) {LASER, 119, 5};

    // Initialize letter game over type
    letterGameOverType = (SpriteType) {LETTER_GAME_OVER, 124, 8};

    // Initialize letter logo type
    letterLogoType = (SpriteType) {LETTER_LOGO, 132, 20};

    // Initialize star type
    starType = (SpriteType) {STAR, 152, 6};
    
    starTypeNumber[0] = 7;
    starTypeNumber[1] = 5;
    starTypeNumber[2] = 6;
    starTypeNumber[3] = 5;
    starTypeNumber[4] = 3;
    starTypeNumber[5] = 4;
}

void init_objects(int playerNum, int asteroidPerPlayer, int laserPerPlayer)
{
    init_entity_types();

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

    objNum = 0;
    shipNum = playerNum;
    statusNum = shipNum;
    asteroidNum = asteroidPerPlayer * shipNum;
    laserNum = laserPerPlayer * shipNum;
    num_letters_game_over = 8;
    num_letters_logo = 9;
    num_letters_push_to_start = 11;
    starNum = 30;

    // Not used anymore
    //int num_objects_to_initialize = shipNum + statusNum + asteroidNum + laserNum + num_letters_game_over + num_letters_logo + num_letters_push_to_start + starNum;
    
    objects = (Object *) malloc(sizeof(Object) * MAX_OBJS);
    dirty_objects = (int *) malloc(sizeof(int) * MAX_OBJS);
    
    memset(objects, 0, sizeof(Object) * MAX_OBJS);
    memset(dirty_objects, 0, sizeof(int) * MAX_OBJS);


    // Initialize the stars
    for (int i = 0; i < NUM_STAR_TYPES; i++) {
        for (int j = 0; j < starTypeNumber[i]; j++) {                               
            objects[objNum] = (Object) {objNum, &starType, i, starPositions[i][2 * j], starPositions[i][2 * j + 1], 0, 0, 0, 1};
            add_dirty_object(&objects[objNum]);
            objNum++;
        }
    }
    for (int i=0; i<laserNum; i++) {
        objects[objNum] = (Object) {objNum, &laserType, 0, 0, 0, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }
    for (int i=0; i<asteroidNum; i++) {
        objects[objNum] = (Object) {objNum, &asteroidTypes[i % NUM_ASTEROID_TYPES], 0, 0, 0, 0, 0, 0, 1};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }
    for (int i=0; i<shipNum; i++) {
        objects[objNum] = (Object) {objNum, &shipTypes[(i) % NUM_SHIP_TYPES], 0, 150 + 50 * i, 400, 0, 0, 1, 1};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }
    for (int i=0; i<statusNum; i++) {
        objects[objNum] = (Object) {objNum, &statusTypes[i % NUM_STATUS_TYPES], 0, WIDTH - 16*(statusNum - i), 0, 0, 0, 1, 1};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    int xGameOverBase = WIDTH / 2 - 32;
    int yGameOverBase = HEIGHT / 2 - 16;
    for (int i=0; i<num_letters_game_over; i++) {
        int xGameOver = xGameOverBase + 16 * (i % 4);
        int yGameOver = yGameOverBase + 16 * (i / 4);
        objects[objNum] = (Object) {objNum, &letterGameOverType, i, xGameOver, yGameOver, 0, 0, 1, 0};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }

    // Positioning of logo
    int xLogoBase = WIDTH / 2 - 16 * 4 - 8;
    int yLogoBase = HEIGHT / 2 - 32;
    for (int i=0; i<num_letters_logo; i++) {
        int xLogo = xLogoBase + 16 * i;
        objects[objNum] = (Object) {objNum, &letterLogoType, i, xLogo, yLogoBase, 0, 0, 1, 1};
        add_dirty_object(&objects[objNum]);
        objNum++;
    }
    // Positioning of push to start
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
}
    
void add_dirty_object(Object* obj)
{
    dirty_objects[obj->id] = 1;

#if DEBUG_WRITE_OBJECT_DIRECT
    write_object(obj);
#endif

}

int move_object(Object* obj, int rot, int speed)
{
    int xNew;
    int yNew;

	int xOffset = 0;
	int yOffset = 0;

	int angle = rot % 4;
	int dir = rot / 4;
	int tmp = -1;

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

	switch(dir) {
		case 1:
			tmp = xOffset;
			xOffset = -yOffset;
			yOffset = tmp;
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

	xNew = obj->xPos + xOffset;
	yNew = obj->yPos + yOffset;

	if (xNew < 0 || xNew + 15 >= WIDTH || yNew < 0 || yNew + 15 >= HEIGHT) {
		return 0;
	}

	obj->xPos = xNew;
	obj->yPos = yNew;

    return 1;
}

int get_rot(Object* Object) {
    bool xFlip = Object->yFlip;
    bool yFlip = Object->xFlip;

    if (!xFlip && !yFlip)
    {
        return Object->localSpriteIdx;
    }
    else if (xFlip && !yFlip)
    {
        return 8 - Object->localSpriteIdx;
    }
    else if (xFlip && yFlip)
    {
        return Object->localSpriteIdx + 8;
    }
    return (16 - Object->localSpriteIdx) % 16;
}

void delete_objects() {
    free(objects);
}

void print_object(Object* obj) {
    printf("id:%d, globalSpriteIdx:%d, localSpriteIdx:%d, xPos:%d, yPos:%d, xFlip:%d, yFlip:%d, enable:%d, priority:%d\n",
    obj->id, obj->type->globalSpriteIdx, obj->localSpriteIdx, obj->xPos, obj->yPos, obj->xFlip, obj->yFlip, obj->enabled, obj->priority);
}
