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
    spriteType* asteroidTypes[3] = {&asteroid1Type, &asteroid2Type, &asteroid3Type};

    objMax = 0;
    shipMax = 1;
    statusMax = 1;
    asteroidMax = 7;
    laserMax = 2;
    
    objs = (object *) malloc(sizeof(object) * (shipMax + statusMax + asteroidMax + laserMax));
    dirtyObjs = (int *) malloc(sizeof(int) * (shipMax + statusMax + asteroidMax + laserMax));

    int cnt = 0;
    for (int i=0; i<shipMax; i++) {
        objs[i] = (object) {objMax++, &shipType, 0, 200, 400, 0, 0, 1, 0};
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
        objs[i] = (object) {objMax++, asteroidTypes[(i - shipMax - statusMax) % 3], 0, xPos, yPos, 0, 0, 1, 0};
        add_dirty_object(&objs[i]);
    }
    cnt += asteroidMax;
    for (int i=cnt; i<cnt + laserMax; i++) {
        objs[i] = (object) {objMax++, &laserType, 0, 0, 0, 0, 0, 0, 0};
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
        return (16 - object->localSpriteIdx) % 16;
    }
    else if (!xFlip && yFlip)
    {
        return 8 - object->localSpriteIdx;
    }
    return object->localSpriteIdx + 8;
}

void delete_objects() {
    free(objs);
}

void print_object(object* obj) {
    printf("id:%d, globalSpriteIdx:%d, localSpriteIdx:%d, xPos:%d, yPos:%d, xFlip:%d, yFlip:%d, enable:%d, priority:%d\n",
    obj->id, obj->type->globalSpriteIdx, obj->localSpriteIdx, obj->xPos, obj->yPos, obj->xFlip, obj->yFlip, obj->enable, obj->priority);
}
