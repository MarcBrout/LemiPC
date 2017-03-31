/*
** loop.c for loop in /home/duhieu_b/Concurrence/PSU_2016_lemipc
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Fri Mar 31 11:25:48 2017 duhieu_b
** Last update Fri Mar 31 11:31:29 2017 duhieu_b
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "game.h"

static void playATurn(int id[2], void *ptrMemShared, t_player *player,
		      struct sembuf sops[NB_SEM])
{
  if (player->dead)
    semop(id[0], &sops[LOOP], 1);
  else
    {
      if (checkDead(player, ptrMemShared))
	{
	  ((int *)ptrMemShared)[player->x + player->y * WIDTH] = 0;
	  player->dead = true;
	  semop(id[0], &sops[LOOP], 1);
	}
      else
	{
	  moveAtRandom(id[1], player, ptrMemShared);
	  semop(id[0], &sops[LOOP], 1);
	  sops[GRAPH].sem_op = 1;
	  semop(id[0], &sops[GRAPH], 1);
	  usleep(10);
	  sops[GRAPH].sem_op = -1;
	  semop(id[0], &sops[GRAPH], 1);
	}
    }
}

void gameLoop(void *ptrMemShared, int id[2], struct sembuf sops[NB_SEM],
	      t_player *player)
{
  while (notGameOver(ptrMemShared))
    {
      semop(id[0], &sops[OVER], 1);
      if (semctl(id[0], LOOP, GETVAL) == player->turn)
	{
	  playATurn(id, ptrMemShared, player, sops);
	}
      else if (graphic)
	{
	  usleep(100000);
	}
      else
	{
	  usleep(1000);
	}
    }
}
