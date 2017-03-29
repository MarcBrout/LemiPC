/*
** order.c for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:05:05 2017 brout_m
** Last update Tue Mar 28 19:06:15 2017 brout_m
*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include "game.h"

bool		getOrder(int msgId, Player me, Map m, bool *ordered)
{
  t_msg		msg;

  *ordered = false;
  if (msgrcv(msgId, &msg, sizeof(t_target), me->team, IPC_NOWAIT) > 0)
    {
        if (msg.target.sender == me->turn)
            return (false);
      *ordered = tryToMoveTo(me, m, &msg.target);
      if (msg.target.lifeTime)
        {
	  --msg.target.lifeTime;
	  msgsnd(msgId, &msg, sizeof(t_target), IPC_NOWAIT);
	  if (!msg.target.lifeTime)
	    return (false);
        }
      return (true);
    }
  return (false);
}

static int distToTarget(Player me, int x, int y)
{
  return ((me->x - x) * (me->x - x) + (me->y - y) * (me->y - y));
}

static void	findTarget(Player me, Map m, Target out)
{
  t_target	closest;
  int		dist;
  int		x;
  int		y;

  x = y = -1;
  closest.dist = -1;
  while (++y < HEIGHT)
    {
      while (++x < WIDTH)
        {
	  if (MAP(m, y, x) && MAP(m, y, x) != me->team &&
	      ((dist = distToTarget(me, x, y)) < closest.dist ||
	       closest.dist == -1))
            {
	      closest.dist = dist;
	      closest.y = y;
	      closest.x = x;
            }
        }
    }
  if (closest.dist != -1)
    *out = closest;
}

void		sendOrder(int msgId, Player me, Map m)
{
  t_msg		msg;

  findTarget(me, m, &msg.target);
  if (msg.target.dist != -1)
    {
      msg.mtype = me->team;
      msg.target.sender = me->turn;
      msg.target.lifeTime = MESSAGE_LIFETIME;
      msgsnd(msgId, &msg, sizeof(t_target), IPC_NOWAIT);
    }
}
