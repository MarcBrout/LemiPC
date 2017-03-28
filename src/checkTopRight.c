/*
** checkTopRight.c for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:01:14 2017 brout_m
** Last update Tue Mar 28 19:02:06 2017 brout_m
*/

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
