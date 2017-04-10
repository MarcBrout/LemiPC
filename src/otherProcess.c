/*
** otherProcess.c for otherProcess in /home/duhieu_b/Concurrence/PSU_2016_lemipc
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Thu Mar 30 17:53:27 2017 duhieu_b
** Last update Fri Mar 31 11:29:23 2017 duhieu_b
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

bool graphic;

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
  gameLoop(ptrMemShared, id, sops, &player);
  sops[QUIT].sem_op = -1;
  semop(id[0], &sops[QUIT], 1);
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] -=1;
  sops[QUIT].sem_op = 1;
  semop(id[0], &sops[QUIT], 1);
}

static int	checkMemory(key_t key, int *memId, void **ptrMemShared, int id[2])
{
  if ((*memId = shmget(key, (HEIGHT * WIDTH + 2) * sizeof(int), 0444)) < 0)
    {
      perror("shmget");
      return (1);
    }
  if ((id[0] = semget(key, NB_SEM, SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  if ((*ptrMemShared = shmat(*memId, NULL, SHM_R | SHM_W)) == (void *) - 1)
    {
      perror("shmat");
      return (1);
    }
  if ((id[1] = msgget(key, SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  return (0);
}

int	otherProcess(key_t key, int memId, int teamNb)
{
  void		*ptrMemShared;
  int		id[2];

  if (checkMemory(key, &memId, &ptrMemShared, id))
    return (1);
  if (((int *)ptrMemShared)[WIDTH * HEIGHT + 1] == WIDTH * HEIGHT)
    {
      dprintf(2, "No room left.\n");
      return (1);
    }
  goToGame(teamNb, id, ptrMemShared);
  return (0);
}
