
#include "direction.h"

Direction getMoveDirection(int xdelta, int ydelta)
{
    if (xdelta < 0 && ydelta == 0)
        return WEST;
    if (xdelta > 0 && ydelta == 0)
        return EAST;
    if (xdelta == 0 && ydelta < 0)
        return NORTH;
    if (xdelta == 0 && ydelta > 0)
        return SOUTH;
    if (xdelta > 0 && ydelta > 0)
        return SOUTHEAST;
    if (xdelta < 0 && ydelta > 0)
        return SOUTHWEST;
    if (xdelta > 0 && ydelta < 0)
        return NORTHEAST;
    if (xdelta < 0 && ydelta < 0)
        return SOUTHWEST;

    return 0; // won't get here
}
Direction getReverseDirection(Direction direction)
{
    switch (direction)
    {
        case WEST:
            return EAST;
        case EAST:
            return WEST;
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case NORTHWEST:
            return SOUTHEAST;
        case SOUTHEAST:
            return NORTHWEST;
        case NORTHEAST:
            return SOUTHWEST;
        case SOUTHWEST:
            return NORTHEAST;
        default:
            return 0; // won't get here
    }
    return 0; // won't get here either
}
int getXNormalFromDirection(Direction direction)
{
    switch(direction)
    {
        case WEST:
        case NORTHWEST:
        case SOUTHWEST:
            return -1;
        case EAST:
        case NORTHEAST:
        case SOUTHEAST:
            return 1;
        case NORTH:
        case SOUTH:
        default:
            return 0;
    }
    return 0;
}
int getYNormalFromDirection(Direction direction)
{
    switch(direction)
    {
        case NORTH:
        case NORTHWEST:
        case NORTHEAST:
            return -1;
        case SOUTH:
        case SOUTHWEST:
        case SOUTHEAST:
            return 1;
        case WEST:
        case EAST:
        default:
            return 0;
    }
    return 0;
}