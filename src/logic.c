/*
** logic.c for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:02:51 2017 brout_m
** Last update Thu Mar 30 17:08:27 2017 brout_m
*/

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
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
      while ((test = rand() % 4) >= 0 && done[test]);
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

bool		isGameOver(Map m, int sem_id)
{
  int		x;
  int		color;
  struct sembuf ops;

  ops.sem_num = OVER;
  ops.sem_flg = color = 0;
  ops.sem_op = 1;
  semop(sem_id, &ops, 1);
  ops.sem_op = x = -1;
  while (++x < HEIGHT * WIDTH)
    {
      if (m[x])
	{
	  if (!color)
	    color = m[x];
	  else if (color != m[x])
	    {
	      semop(sem_id, &ops, 1);
	      return (false);
	    }
	}
    }
  m[WIDTH * HEIGHT] = 1;
  semop(sem_id, &ops, 1);
  return (true);
}
