/*
** logic.c for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:02:51 2017 brout_m
** Last update Tue Mar 28 19:08:42 2017 brout_m
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

static moveTo const moveToTab[4] =
  {
    moveTop,
    moveRight,
    moveBot,
    moveLeft
  };

static isCell const isCellTab[8] =
  {
    isTop,
    isTopRight,
    isRight,
    isBotRight,
    isBot,
    isBotLeft,
    isLeft,
    isTopLeft
  };

static bool	addColorToTab(Player const me, int color, int teamBoard[8])
{
  int		i;

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

bool	checkDead(Player me, Map m)
{
  int	cellPlayerTeam;
  int	teamBoard[8];
  int	i;

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

bool	allDone(bool const done[4])
{
  int	i;

  i = 0;
  while (i < 4)
    {
      if (!done[i])
	return (false);
      ++i;
    }
  return (true);
}

void			moveAtRandom(int msgId, Player me, Map m)
{
  struct s_player	meCpy;
  int			test;
  bool			done[4];
  bool			moved;
  bool			ordered;

  memset(done, 0, sizeof(bool) * 4);
  ordered = getOrder(msgId, me, m, &moved);
  while (!moved && !allDone(done))
    {
        printf("test");
      while ((test = rand() % 4) >= 0 && done[test]);
        printf("test2");
      meCpy = *me;
      if (moveToTab[test](&meCpy, m) && !checkDead(&meCpy, m))
        {
	  MAP(m, me->y, me->x) = 0;
	  *me = meCpy;
	  MAP(m, me->y, me->x) = me->team;
	  break;
        }
      done[test] = true;
    }
  if (!ordered)
    sendOrder(msgId, me, m);
}

bool	isGameOver(Map const m)
{
  int	y;
  int	x;
  int	color;

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
