/*
** go.c for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:02:16 2017 brout_m
** Last update Tue Mar 28 19:02:40 2017 brout_m
*/

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
