#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "object.h"

void init_objects(void)
{
    ship1Type = (spriteType) {SHIP, 0, 5};
    ship2Type = (spriteType) {SHIP, 5, 5};
    statusType = (spriteType) {STATUS, 10, 4};
    asteroid1Type = (spriteType) {ASTEROID, 14, 11};
    asteroid2Type = (spriteType) {ASTEROID, 25, 11};
    asteroid3Type = (spriteType) {ASTEROID, 36, 16};
    laserType = (spriteType) {LASER, 52, 5};
    letterType = (spriteType) {LETTER, 57, 8};
    starType = (spriteType) {STAR, 65, 6};

    spriteType* shipTypes[2] = {&ship1Type, &ship2Type};
    spriteType* asteroidTypes[3] = {&asteroid1Type, &asteroid2Type, &asteroid3Type};
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
    asteroidMax = 2;
    laserMax = 3 * shipMax;
    letterMax = 8;
    starMax = 30;
    
    objs = (object *) malloc(sizeof(object) * (shipMax + statusMax + asteroidMax + laserMax + letterMax + starMax));
    dirtyObjs = (int *) malloc(sizeof(int) * (shipMax + statusMax + asteroidMax + laserMax + letterMax + starMax));

    for (int i=0; i<shipMax; i++) {
        objs[objMax] = (object) {objMax, shipTypes[i % 2], 0, 150 + 50*i, 400, 0, 0, 1, 1};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<statusMax; i++) {
        objs[objMax] = (object) {objMax, &statusType, 0, WIDTH - 16*(i+1), 0, 0, 0, 1, 1};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<asteroidMax; i++) {
        objs[objMax] = (object) {objMax, asteroidTypes[i % 3], 0, 0, 0, 0, 0, 1, 1};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<laserMax; i++) {
        objs[objMax] = (object) {objMax, &laserType, 0, 0, 0, 0, 0, 0, 1};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    int xLetterBase = WIDTH / 2 - 32;
    int yLetterBase = HEIGHT / 2 - 16;
    for (int i=0; i<letterMax; i++) {
        int xLetter = xLetterBase + 16 * (i % 4);
        int yLetter = yLetterBase + 16 * (i / 4);
        objs[objMax] = (object) {objMax, &letterType, i, xLetter, yLetter, 0, 0, 0, 1};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<star1Max; i++) {
        objs[objMax] = (object) {objMax, &starType, 0, star1Pos[2*i], star1Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<star2Max; i++) {
        objs[objMax] = (object) {objMax, &starType, 1, star2Pos[2*i], star2Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<star3Max; i++) {
        objs[objMax] = (object) {objMax, &starType, 2, star3Pos[2*i], star3Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<star4Max; i++) {
        objs[objMax] = (object) {objMax, &starType, 3, star4Pos[2*i], star4Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<star5Max; i++) {
        objs[objMax] = (object) {objMax, &starType, 4, star5Pos[2*i], star5Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
    for (int i=0; i<star6Max; i++) {
        objs[objMax] = (object) {objMax, &starType, 5, star6Pos[2*i], star6Pos[2*i + 1], 0, 0, 1, 0};
        add_dirty_object(&objs[objMax]);
        objMax++;
    }
}

void add_dirty_object(object* obj) {
    dirtyObjs[obj->id] = 1;
}

int move_object(object* obj, int rot, int speed) {
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

int get_rot(object* object) {
    bool xFlip = object->xFlip;
    bool yFlip = object->yFlip;

    if (!xFlip && !yFlip)
    {
        return object->localSpriteIdx;
    }
    else if (xFlip && !yFlip)
    {
        return 8 - object->localSpriteIdx;
    }
    else if (xFlip && yFlip)
    {
        return object->localSpriteIdx + 8;
    }
    return (16 - object->localSpriteIdx) % 16;
}

void delete_objects() {
    free(objs);
}

void print_object(object* obj) {
    printf("id:%d, globalSpriteIdx:%d, localSpriteIdx:%d, xPos:%d, yPos:%d, xFlip:%d, yFlip:%d, enable:%d, priority:%d\n",
    obj->id, obj->type->globalSpriteIdx, obj->localSpriteIdx, obj->xPos, obj->yPos, obj->xFlip, obj->yFlip, obj->enable, obj->priority);
}
