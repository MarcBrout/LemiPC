#include "game.h"

int isTop(Player me, Map m)
{
    if (me->y == 0)
        return (-1);
    return (TOP(me, m));
}

int isTopRight(Player me, Map m)
{
    if (me->y == 0 || me->x == WIDTH - 1)
        return (-1);
    return (TOPR(me, m));
}

int isRight(Player me, Map m)
{
    if (me->y == WIDTH + 1)
        return (-1);
    return (RIGHT(me, m));
}

int isBotRight(Player me, Map m)
{
    if (me->y == HEIGHT - 1 || me->x == WIDTH - 1)
        return (-1);
    return (BOTR(me, m));
}

