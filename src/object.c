#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "object.h"

void init_objects(void)
{
    shipType = (spriteType) {SHIP, 0, 5};
    statusType = (spriteType) {STATUS, 10, 4};
    asteroid1Type = (spriteType) {ASTEROID, 14, 11};
    asteroid2Type = (spriteType) {ASTEROID, 25, 11};
    asteroid3Type = (spriteType) {ASTEROID, 36, 16};
    laserType = (spriteType) {LASER, 52, 5};
    letterType = (spriteType) {LETTER, 57, 8};
    starType = (spriteType) {STAR, 65, 6};

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
    shipMax = 1;
    statusMax = 1;
    asteroidMax = 1;
    laserMax = 2;
    letterMax = 8;
    starMax = 30;
    
    objs = (object *) malloc(sizeof(object) * (shipMax + statusMax + asteroidMax + laserMax + letterMax + starMax));
    dirtyObjs = (int *) malloc(sizeof(int) * (shipMax + statusMax + asteroidMax + laserMax + letterMax + starMax));

    int cnt = 0;
    for (int i=0; i<shipMax; i++) {
        objs[i] = (object) {objMax++, &shipType, 0, 200, 400, 0, 0, 1, 1};
        add_dirty_object(&objs[i]);
    }
    cnt += shipMax;
    for (int i=cnt; i<cnt + statusMax; i++) {
        objs[i] = (object) {objMax++, &statusType, 0, WIDTH - 16, 0, 0, 0, 1, 1};
        add_dirty_object(&objs[i]);
    }
    cnt += statusMax;
    for (int i=cnt; i<cnt + asteroidMax; i++) {
        bool seek = 1;
        int xPos = 0;
        int yPos = 0;
        while (seek) {
            xPos = rand() % (WIDTH - 16);
            yPos = rand() % (HEIGHT - 16);
            if (xPos <= 100 || xPos >= 300 || yPos <= 300 || yPos >= 500) {
                seek = 0;
            }
        }
        objs[i] = (object) {objMax++, asteroidTypes[(i - shipMax - statusMax) % 3], 0, xPos, yPos, 0, 0, 1, 1};
        add_dirty_object(&objs[i]);
    }
    cnt += asteroidMax;
    for (int i=cnt; i<cnt + laserMax; i++) {
        objs[i] = (object) {objMax++, &laserType, 0, 0, 0, 0, 0, 0, 1};
        add_dirty_object(&objs[i]);
    }
    cnt += laserMax;
    int xLetterBase = WIDTH / 2 - 32;
    int yLetterBase = HEIGHT / 2 - 16;
    for (int i=cnt; i<cnt + letterMax; i++) {
        int num = i - shipMax - statusMax - asteroidMax - laserMax;
        int xLetter = xLetterBase + 16 * (num % 4);
        int yLetter = yLetterBase + 16 * (num / 4);
        objs[i] = (object) {objMax++, &letterType, num, xLetter, yLetter, 0, 0, 0, 1};
        add_dirty_object(&objs[i]);
    }
    cnt += letterMax;
    int starIdx = 0;
    for (int i=cnt; i<cnt + star1Max; i++) {
        objs[i] = (object) {objMax++, &starType, 0, star1Pos[2*starIdx], star1Pos[2*starIdx + 1], 0, 0, 1, 0};
        starIdx++;
        add_dirty_object(&objs[i]);
    }
    cnt += star1Max;
    starIdx = 0;
    for (int i=cnt; i<cnt + star2Max; i++) {
        objs[i] = (object) {objMax++, &starType, 1, star2Pos[2*starIdx], star2Pos[2*starIdx + 1], 0, 0, 1, 0};
        starIdx++;
        add_dirty_object(&objs[i]);
    }
    cnt += star2Max;
    starIdx = 0;
    for (int i=cnt; i<cnt + star3Max; i++) {
        objs[i] = (object) {objMax++, &starType, 2, star3Pos[2*starIdx], star3Pos[2*starIdx + 1], 0, 0, 1, 0};
        starIdx++;
        add_dirty_object(&objs[i]);
    }
    cnt += star3Max;
    starIdx = 0;
    for (int i=cnt; i<cnt + star4Max; i++) {
        objs[i] = (object) {objMax++, &starType, 3, star4Pos[2*starIdx], star4Pos[2*starIdx + 1], 0, 0, 1, 0};
        starIdx++;
        add_dirty_object(&objs[i]);
    }
    cnt += star4Max;
    starIdx = 0;
    for (int i=cnt; i<cnt + star5Max; i++) {
        objs[i] = (object) {objMax++, &starType, 4, star5Pos[2*starIdx], star5Pos[2*starIdx + 1], 0, 0, 1, 0};
        starIdx++;
        add_dirty_object(&objs[i]);
    }
    cnt += star5Max;
    starIdx = 0;
    for (int i=cnt; i<cnt + star6Max; i++) {
        objs[i] = (object) {objMax++, &starType, 5, star6Pos[2*starIdx], star6Pos[2*starIdx + 1], 0, 0, 1, 0};
        starIdx++;
        add_dirty_object(&objs[i]);
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
