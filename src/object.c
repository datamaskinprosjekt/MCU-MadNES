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
    asteroidMax = 1;
    laserMax = 2;
    
    shipObjs = (object *) malloc(sizeof(object) * shipMax);
    statusObjs = (object *) malloc(sizeof(object) * statusMax);
    asteroidObjs = (object *) malloc(sizeof(object) * asteroidMax);
    laserObjs = (object *) malloc(sizeof(object) * laserMax);

    for (int i=0; i<shipMax; i++) {
        shipObjs[i] = (object) {objMax++, &shipType, shipType.globalSpriteIdx, 200, 400, 1, 0, 1, 0};
        add_dirty_object(&shipObjs[i]);
    }
    for (int i=0; i<statusMax; i++) {
        statusObjs[i] = (object) {objMax++, &statusType, statusType.globalSpriteIdx, WIDTH - 16, HEIGHT, 0, 0, 1, 1};
        add_dirty_object(&statusObjs[i]);
    }
    for (int i=0; i<asteroidMax; i++) {
        asteroidObjs[i] = (object) {objMax++, &asteroidType, asteroidType.globalSpriteIdx, 450, 150, 0, 0, 1, 0};
        add_dirty_object(&asteroidObjs[i]);
    }
    for (int i=0; i<laserMax; i++) {
        laserObjs[i] = (object) {objMax++, &laserType, laserType.globalSpriteIdx, 0, 0, 0, 0, 0, 0};
        add_dirty_object(&laserObjs[i]);
    }
}

void add_dirty_object(object* obj) {
    //
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
    free(shipObjs);
    free(statusObjs);
    free(asteroidObjs);
    free(laserObjs);
}

void print_object(object* obj) {
    printf("id:%d, localSpriteIdx:%d, xPos:%d, yPos:%d, xFlip:%d, yFlip:%d, enable:%d, priority:%d\n",
    obj->id, obj->localSpriteIdx, obj->xPos, obj->yPos, obj->xFlip, obj->yFlip, obj->enable, obj->priority);
}
