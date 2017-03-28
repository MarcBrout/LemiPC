#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

bool tryToTop(Player me, Map m, Target const t)
{
    struct s_player meCpy;

    meCpy = *me;
    if (me->y > t->y && !MAP(m, me->y - 1, me->x))
    {
        --meCpy.y;
        if (checkDead(&meCpy, m))
            return (false);
        MAP(m, me->y, me->x) = 0;
        --me->y;
        MAP(m, me->y, me->x) = me->team;
        return (true);
    }
    return (false);
}

bool tryToRight(Player me, Map m, Target const t)
{
    struct s_player meCpy;

    meCpy = *me;
    if (me->x < t->x && !MAP(m, me->y, me->x + 1))
    {
        ++meCpy.x;
        if (checkDead(&meCpy, m))
            return (false);
        MAP(m, me->y, me->x) = 0;
        ++me->x;
        MAP(m, me->y, me->x) = me->team;
        return (true);
    }
    return (false);
}

bool tryToBot(Player me, Map m, Target const t)
{
    struct s_player meCpy;

    meCpy = *me;
    if (me->y < t->y && !MAP(m, me->y + 1, me->x))
    {
        ++meCpy.y;
        if (checkDead(&meCpy, m))
            return (false);
        MAP(m, me->y, me->x) = 0;
        ++me->y;
        MAP(m, me->y, me->x) = me->team;
        return (true);
    }
    return (false);
}

bool tryToLeft(Player me, Map m, Target const t)
{
    struct s_player meCpy;

    meCpy = *me;
    if (me->x > t->x && !MAP(m, me->y, me->x - 1))
    {
        --meCpy.x;
        if (checkDead(&meCpy, m))
            return (false);
        MAP(m, me->y, me->x) = 0;
        --me->x;
        MAP(m, me->y, me->x) = me->team;
        return (true);
    }
    return (false);
}

static tryTo const tryToTab[4] =
        {
                tryToTop,
                tryToRight,
                tryToBot,
                tryToLeft
        };

bool tryToMoveTo(Player me, Map m, Target const target)
{
    bool done[4];
    int test;

    memset(done, 0, sizeof(bool) * 4);
    while (!allDone(done))
    {
        while ((test = rand() % 4) >= 0 && done[test]);
        if (tryToTab[test](me, m, target))
            return (true);
    }
    return (false);
}