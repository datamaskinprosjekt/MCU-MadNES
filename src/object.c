#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "object.h"

void init_objects(void)
{
    ship1Type = (SpriteType) {SHIP, 0, 5};
    ship2Type = (SpriteType) {SHIP, 5, 5};
    ship3Type = (SpriteType) {SHIP, 10, 5};
    ship4Type = (SpriteType) {SHIP, 15, 5};
    ship5Type = (SpriteType) {SHIP, 20, 5};
    ship6Type = (SpriteType) {SHIP, 25, 5};
    ship7Type = (SpriteType) {SHIP, 30, 5};
    ship8Type = (SpriteType) {SHIP, 35, 5};
    ship9Type = (SpriteType) {SHIP, 40, 5};
    status1Type = (SpriteType) {STATUS, 45, 4};
    status2Type = (SpriteType) {STATUS, 49, 4};
    status3Type = (SpriteType) {STATUS, 53, 4};
    status4Type = (SpriteType) {STATUS, 57, 4};
    status5Type = (SpriteType) {STATUS, 61, 4};
    status6Type = (SpriteType) {STATUS, 65, 4};
    status7Type = (SpriteType) {STATUS, 69, 4};
    status8Type = (SpriteType) {STATUS, 73, 4};
    status9Type = (SpriteType) {STATUS, 77, 4};
    asteroid1Type = (SpriteType) {ASTEROID, 81, 11};
    asteroid2Type = (SpriteType) {ASTEROID, 92, 11};
    asteroid3Type = (SpriteType) {ASTEROID, 103, 16};
    laserType = (SpriteType) {LASER, 119, 5};
    letterType = (SpriteType) {LETTER, 125, 8};
    starType = (SpriteType) {STAR, 132, 6};

    SpriteType* shipTypes[9] = {&ship1Type, &ship2Type, &ship3Type, &ship4Type, &ship5Type, &ship6Type, &ship7Type, &ship8Type, &ship9Type};
    SpriteType* statusTypes[9] = {&status1Type, &status2Type, &status3Type, &status4Type, &status5Type, &status6Type, &status7Type, &status8Type, &status9Type};
    SpriteType* asteroidTypes[3] = {&asteroid1Type, &asteroid2Type, &asteroid3Type};
    int star1Max = 7;
    int star2Max = 5;
    int star3Max = 6;
    int star4Max = 5;
    int star5Max = 3;
    int star6Max = 4;
    int star1Pos[2 * 7] = {62,136, 227,380, 283,174, 321,278, 408,88, 488,252, 582,174};
    int star2Pos[2 * 5] = {122,112, 158,273, 392,113, 469,380, 527,262};
    int star3Pos[2 * 6] = {85,331, 124,53, 329,93, 416,305, 477,128, 530,411};
    int star4Pos[2 * 5] = {86,419, 107,205, 252,64, 408,224, 547,40};
    int star5Pos[2 * 3] = {45,103, 230,146, 309,417};
    int star6Pos[2 * 4] = {258,209, 355,235, 489,63, 535,311};

    objMax = 0;
    shipMax = 2;
    statusMax = shipMax;
    asteroidMax = 1 * shipMax;
    laserMax = 3 * shipMax;
    letterMax = 8;
    starMax = 30;

    int num_objects_to_initialize = shipMax + statusMax + asteroidMax + laserMax + letterMax + starMax;
    
    objects = (Object *) malloc(sizeof(Object) * num_objects_to_initialize);
    dirty_objects = (int *) malloc(sizeof(int) * num_objects_to_initialize);

    for (int i=0; i<shipMax; i++) {
        objects[objMax] = (Object) {objMax, shipTypes[i % 9], 0, 150 + 50*i, 400, 0, 0, 1, 1};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<statusMax; i++) {
        objects[objMax] = (Object) {objMax, statusTypes[i % 9], 0, WIDTH - 16*(shipMax-i), 0, 0, 0, 1, 1};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<asteroidMax; i++) {
        objects[objMax] = (Object) {objMax, asteroidTypes[i % 3], 0, 0, 0, 0, 0, 1, 1};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<laserMax; i++) {
        objects[objMax] = (Object) {objMax, &laserType, 0, 0, 0, 0, 0, 0, 1};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    int xLetterBase = WIDTH / 2 - 32;
    int yLetterBase = HEIGHT / 2 - 16;
    for (int i=0; i<letterMax; i++) {
        int xLetter = xLetterBase + 16 * (i % 4);
        int yLetter = yLetterBase + 16 * (i / 4);
        objects[objMax] = (Object) {objMax, &letterType, i, xLetter, yLetter, 0, 0, 0, 1};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<star1Max; i++) {
        objects[objMax] = (Object) {objMax, &starType, 0, star1Pos[2*i], star1Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<star2Max; i++) {
        objects[objMax] = (Object) {objMax, &starType, 1, star2Pos[2*i], star2Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<star3Max; i++) {
        objects[objMax] = (Object) {objMax, &starType, 2, star3Pos[2*i], star3Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<star4Max; i++) {
        objects[objMax] = (Object) {objMax, &starType, 3, star4Pos[2*i], star4Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<star5Max; i++) {
        objects[objMax] = (Object) {objMax, &starType, 4, star5Pos[2*i], star5Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
    for (int i=0; i<star6Max; i++) {
        objects[objMax] = (Object) {objMax, &starType, 5, star6Pos[2*i], star6Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objects[objMax]);
        objMax++;
    }
}

void add_dirty_object(Object* obj) {
    dirty_objects[obj->id] = 1;
}

int move_object(Object* obj, int rot, int speed) {
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

	int xNew = obj->xPos + xOffset;
	int yNew = obj->yPos + yOffset;

	if (xNew < 0 || xNew + 15 >= WIDTH || yNew < 0 || yNew + 15 >= HEIGHT) {
		return 0;
	}

	obj->xPos = xNew;
	obj->yPos = yNew;
    return 1;
}

int get_rot(Object* Object) {
    bool xFlip = Object->xFlip;
    bool yFlip = Object->yFlip;

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
