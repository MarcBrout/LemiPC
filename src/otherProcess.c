/*
** otherProcess.c for otherProcess in /home/duhieu_b/Concurrence/PSU_2016_lemipc
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Thu Mar 30 17:53:27 2017 duhieu_b
** Last update Thu Mar 30 18:21:32 2017 duhieu_b
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "game.h"

static void initSems(struct sembuf sops[NB_SEM],
		     int sem_id, void *ptrMemShared)
{
  sops[LOOP].sem_num = LOOP;
  sops[LOOP].sem_flg = 0;
  sops[QUIT].sem_num = QUIT;
  sops[QUIT].sem_flg = 0;
  sops[QUIT].sem_op = -1;
  semop(sem_id, &sops[QUIT], 1);
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] += 1;
  sops[QUIT].sem_op = 1;
  semop(sem_id, &sops[QUIT], 1);
}

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

static void goToGame(int teamNb, int id[2], void *ptrMemShared)
{
  int		playerMax;
  struct sembuf sops[NB_SEM];
  t_player	player;

  initSems(sops, id[0], ptrMemShared);
  playerMax = ((int *)ptrMemShared)[WIDTH * HEIGHT + 1];
  semctl(id[0], LOOP, SETVAL, playerMax);
  putPlayerInMap(teamNb, ptrMemShared, &player, playerMax);
  sops[LOOP].sem_op = -1;
  sops[GRAPH].sem_num = GRAPH;
  sops[OVER].sem_num = OVER;
  sops[GRAPH].sem_flg = sops[OVER].sem_flg = sops[OVER].sem_op = 0;
  while (notGameOver(ptrMemShared))
    {
      semop(id[0], &sops[OVER], 1);
      if (semctl(id[0], LOOP, GETVAL) == player.turn)
	playATurn(id, ptrMemShared, &player, sops);
      else
	usleep(10);
    }
  sops[QUIT].sem_op = -1;
  semop(id[0], &sops[QUIT], 1);
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] -=1;
  sops[QUIT].sem_op = 1;
  semop(id[0], &sops[QUIT], 1);
}

int	otherProcess(key_t key, int memId, int teamNb)
{
  void		*ptrMemShared;
  int		id[2];

  if ((memId = shmget(key, (HEIGHT * WIDTH + 2) * sizeof(int), 0444)) < 0)
    {
      perror("shmget");
      return (1);
    }
  if ((id[0] = semget(key, NB_SEM, SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  if ((ptrMemShared = shmat(memId, NULL, SHM_R | SHM_W)) == (void *) - 1)
    {
      perror("shmat");
      return (1);
    }
  if ((id[1] = msgget(key, SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  goToGame(teamNb, id, ptrMemShared);
  return (0);
}
