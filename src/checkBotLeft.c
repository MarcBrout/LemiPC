/*
** checkBotLeft.c for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:00:27 2017 brout_m
** Last update Tue Mar 28 19:01:08 2017 brout_m
*/

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
