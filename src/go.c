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
    if (me->y == HEIGHT - 1)
        return (-1);
    return (BOT(me, m));
}

int isRight(Player me, Map m)
{
    if (me->y == WIDTH + 1)
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
    if (me->y == 0 || me->x == WIDTH - 1)
        return (-1);
    return (TOPR(me, m));
}

int isBotRight(Player me, Map m)
{
    if (me->y == HEIGHT - 1 || me->x == WIDTH - 1)
        return (-1);
    return (BOTR(me, m));
}

int isBotLeft(Player me, Map m)
{
    if (me->y == HEIGHT - 1 || me->x == 0)
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

bool addColorToTab(Player me, int color, int teamBoard[8])
{
    int i;

    i = 0;
    while (i < 8)
    {
        if (!teamBoard[i])
        {
            teamBoard[i] = color;
            break;
        }
        else if (teamBoard[i] == color && me->team != color)
            return (true);
        ++i;
    }
    return (false);
}

bool checkDead(Player me, Map m)
{
    int cellPlayerTeam;
    int teamBoard[8];
    int i;

    i = 0;
    memset(teamBoard, 0, sizeof(int) * 8);
    while (i < 8)
    {
        if ((cellPlayerTeam = isCellTab[i](me, m)) > 0)
        {
            if (addColorToTab(me, cellPlayerTeam, teamBoard))
                return (true);
        }
        ++i;
    }
    return (false);
}

bool allDone(bool done[8])
{
    int i;

    i = 0;
    while (i < 8)
    {
        if (!done[i])
            return (false);
        ++i;
    }
    return (true);
}

static moveTo moveToTab[4] = {
        moveTop,
        moveRight,
        moveBot,
        moveLeft
};

void moveAtRandom(Player me, Map m)
{
    struct s_player meCpy;
    int test;
    bool done[4];

    memset(done, 0, sizeof(bool) * 4);
    while (!allDone(done))
    {
        while ((test = rand() % 4) >= 0 && done[test]);
        meCpy = *me;
        if (moveToTab[test](&meCpy, m) && !checkDead(&meCpy, m))
        {
            *me = meCpy;
            break;
        }
        done[test] = true;
    }
}

bool isAliveAndPlay(Player me, Map m)
{
    if (checkDead(me, m))
    {
        MAP(m, me->y, me->x) = 0;
        return (false);
    }
    moveAtRandom(me, m);
    return (true);
}

bool isGameOver(Map m)
{
    int y;
    int x;
    int color;

    y = x = color = 0;
    while (y < HEIGHT)
    {
        while (x < WIDTH)
        {
            if (m[y * HEIGHT + x])
            {
                if (!color)
                    color = m[y * HEIGHT + x];
                else if (color != m[y * HEIGHT + x])
                    return (false);
            }
            ++x;
        }
        ++y;
    }
    return (true);
}