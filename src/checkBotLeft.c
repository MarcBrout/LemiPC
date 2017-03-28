#include "game.h"

int isLeft(Player me, Map m)
{
    if (me->x == 0)
        return (-1);
    return (LEFT(me, m));
}

int isBot(Player me, Map m)
{
    if (me->y == HEIGHT - 1)
        return (-1);
    return (BOT(me, m));
}

int isTopLeft(Player me, Map m)
{
    if (me->y == 0 || me->x == 0)
        return (-1);
    return (TOPL(me, m));
}

int isBotLeft(Player me, Map m)
{
    if (me->y == HEIGHT - 1 || me->x == 0)
        return (-1);
    return (BOTL(me, m));
}