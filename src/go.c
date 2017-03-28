#include <stdbool.h>
#include "game.h"

bool moveTop(Player me, Map m)
{
    if (!isTop(me, m))
    {
        --me->y;
        return (true);
    }
    return (false);
}

bool moveRight(Player me, Map m)
{
    if (!isRight(me, m))
    {
        ++me->x;
        return (true);
    }
    return (false);
}

bool moveBot(Player me, Map m)
{
    if (!isBot(me, m))
    {
        ++me->y;
        return (true);
    }
    return (false);
}

bool moveLeft(Player me, Map m)
{
    if (!isLeft(me, m))
    {
        --me->x;
        return (true);
    }
    return (false);
}