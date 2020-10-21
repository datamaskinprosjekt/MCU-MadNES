#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "object.h"

void init_objects(void)
{
    shipType = (spriteType) {SHIP, 0, 5};
    statusType = (spriteType) {STATUS, 5, 4};
    asteroidType = (spriteType) {ASTEROID, 9, 11};
    laserType = (spriteType) {LASER, 20, 5};

    objMax = 0;
    shipMax = 1;
    statusMax = 1;
    asteroidMax = 2;
    laserMax = 2;
    
    shipObjs = (object *) malloc(sizeof(object) * shipMax);
    statusObjs = (object *) malloc(sizeof(object) * statusMax);
    asteroidObjs = (object *) malloc(sizeof(object) * asteroidMax);
    laserObjs = (object *) malloc(sizeof(object) * laserMax);

    for (int i=0; i<shipMax; i++) {
        shipObjs[i] = (object) {objMax++, &shipType, 2, 200, 100, 1, 0, 1, 1};
    }
    for (int i=0; i<statusMax; i++) {
        statusObjs[i] = (object) {objMax++, &statusType, 0, 0, 0, 0, 0, 1, 0};
    }
    for (int i=0; i<asteroidMax; i++) {
        asteroidObjs[i] = (object) {objMax++, &asteroidType, 0, 0, 0, 0, 0, 1, 1};
    }
    for (int i=0; i<laserMax; i++) {
        laserObjs[i] = (object) {objMax++, &laserType, 0, 0, 0, 0, 0, 0, 1};
    }
}

void add_dirty_object(object* obj) {
    //
}

void delete_objects() {
    free(shipObjs);
    free(statusObjs);
    free(asteroidObjs);
    free(laserObjs);
}

void print_object(object* obj) {
    printf("id:%d, localSpriteIdx:%d, xPos:%d, yPos:%d, xFlip:%d, yFlip:%d, enable:%d, priority:%d\n",
    obj->id, obj->localSpriteIdx, obj->xPos, obj->yPos, obj->xFlip, obj->yFlip, obj->enable, obj->priority);
}
