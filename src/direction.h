#ifndef DIRECTION_H
#define DIRECTION_H

#include "definitions.h"

typedef enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTHEAST,
    SOUTHEAST,
    NORTHWEST,
    SOUTHWEST
} Direction;

Direction getMoveDirection(int xdelta, int ydelta);
Direction getReverseDirection(Direction direction);
int getXNormalFromDirection(Direction direction);
int getYNormalFromDirection(Direction direction);

#endif