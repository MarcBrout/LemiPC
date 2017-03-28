#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

int isTop(Player me, Map m)
{
    if (me->y == 0)
        return (-1);
    return (TOP(me, m));
}

int isLeft(Player me, Map m)
{
    if (me->x == 0)
        return (-1);
    return (LEFT(me, m));
}

int isBot(Player me, Map m)
{
    if (me->y == HEIGHT(m) - 1)
        return (-1);
    return (BOT(me, m));
}

int isRight(Player me, Map m)
{
    if (me->y == WIDTH(m) + 1)
        return (-1);
    return (RIGHT(me, m));
}

int isTopLeft(Player me, Map m)
{
    if (me->y == 0 || me->x == 0)
        return (-1);
    return (TOPL(me, m));
}

int isTopRight(Player me, Map m)
{
    if (me->y == 0 || me->x == WIDTH(m) - 1)
        return (-1);
    return (TOPR(me, m));
}

int isBotRight(Player me, Map m)
{
    if (me->y == HEIGHT(m) - 1 || me->x == WIDTH(m) - 1)
        return (-1);
    return (BOTR(me, m));
}

int isBotLeft(Player me, Map m)
{
    if (me->y == HEIGHT(m) - 1 || me->x == 0)
        return (-1);
    return (BOTL(me, m));
}

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

bool moveTopLeft(Player me, Map m)
{
    if (!isTopLeft(me, m))
    {
        --me->x;
        --me->y;
        return (true);
    }
    return (false);
}

bool moveTopRight(Player me, Map m)
{
    if (!isTopRight(me, m))
    {
        ++me->x;
        --me->y;
        return (true);
    }
    return (false);
}

bool moveBotLeft(Player me, Map m)
{
    if (!isBotLeft(me, m))
    {
        --me->x;
        ++me->y;
        return (true);
    }
    return (false);
}

bool moveBotRight(Player me, Map m)
{
    if (!isBotRight(me, m))
    {
        ++me->x;
        ++me->y;
        return (true);
    }
    return (false);
}

static isCell isCellTab[8] = {
        isTop,
        isTopRight,
        isRight,
        isBotRight,
        isBot,
        isBotLeft,
        isLeft,
        isTopLeft
};

bool checkDead(Player me, Map m, int *teamBoard)
{
    int cellPlayerTeam;
    int i;

    i = 0;
    memset(teamBoard, 0, sizeof(int) * m->nbTeam);
    while (i < 8)
    {
        if ((cellPlayerTeam = isCellTab[i](me, m)) > 0)
        {
            ++teamBoard[cellPlayerTeam - 1];
            if (teamBoard[cellPlayerTeam - 1] > 1)
                return (true);
        }
        ++i;
    }
    return (false);
}

int play(int sem, )