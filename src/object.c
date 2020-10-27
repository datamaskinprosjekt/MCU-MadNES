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
    
    objs = (object *) malloc(sizeof(object) * (shipMax + statusMax + asteroidMax + laserMax));
    dirtyObjs = (object *) malloc(sizeof(object) * (shipMax + statusMax + asteroidMax + laserMax));

    for (int i=0; i<shipMax; i++) {
        objs[i] = (object) {objMax++, &shipType, shipType.globalSpriteIdx, 200, 400, 1, 0, 1, 0};
        add_dirty_object(&objs[i]);
    }
    for (int i=0; i<statusMax; i++) {
        objs[i] = (object) {objMax++, &statusType, statusType.globalSpriteIdx, WIDTH - 16, HEIGHT, 0, 0, 1, 1};
        add_dirty_object(&objs[i]);
    }
    for (int i=0; i<asteroidMax; i++) {
        objs[i] = (object) {objMax++, &asteroidType, asteroidType.globalSpriteIdx, 450, 150, 0, 0, 1, 0};
        add_dirty_object(&objs[i]);
    }
    for (int i=0; i<laserMax; i++) {
        objs[i] = (object) {objMax++, &laserType, laserType.globalSpriteIdx, 0, 0, 0, 0, 0, 0};
        add_dirty_object(&objs[i]);
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
    free(objs);
}

void print_object(object* obj) {
    printf("id:%d, localSpriteIdx:%d, xPos:%d, yPos:%d, xFlip:%d, yFlip:%d, enable:%d, priority:%d\n",
    obj->id, obj->localSpriteIdx, obj->xPos, obj->yPos, obj->xFlip, obj->yFlip, obj->enable, obj->priority);
}
