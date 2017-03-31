/*
** firstProcess.c for firstProcess in /home/duhieu_b/Concurrence/PSU_2016_lemipc
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Thu Mar 30 17:56:08 2017 duhieu_b
** Last update Thu Mar 30 18:18:22 2017 duhieu_b
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

static void	initMemory(void *ptrMemShared, int sem_id)
{
  memset(ptrMemShared, 0, (WIDTH * HEIGHT + 2) * sizeof(int));
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] = 1;
  semctl(sem_id, LOOP, SETVAL, 1);
  semctl(sem_id, OVER, SETVAL, 0);
  semctl(sem_id, GRAPH, SETVAL, 1);
  semctl(sem_id, QUIT, SETVAL, 1);
}

static void	testStartGame(bool *start, void *ptrMemShared,
			      int sem_id)
{
  if (!(*start) && isTeams(ptrMemShared))
    *start = true;
  if (semctl(sem_id, GRAPH, GETVAL) == 1)
    {
      if (graphic)
      {
        displayMap(ptrMemShared);
        usleep(50000);
      }
      else
        usleep(500);
    }
}

static void	doingStuff(int sem_id, int msg_id,
			   void *ptrMemShared, t_player *player)
{
  if (semctl(sem_id, LOOP, GETVAL) == 1)
    {
      if (!player->dead)
	{
	  if (!checkDead(player, ptrMemShared))
	    moveAtRandom(msg_id, player, ptrMemShared);
	  else
	    {
	      ((int *)ptrMemShared)[player->x + player->y * WIDTH] = 0;
	      player->dead = true;
	    }
      if (graphic)
      {
        displayMap(ptrMemShared);
        usleep(100000);
      }
      else
      {
        usleep(500);
      }
	}
      semctl(sem_id, LOOP, SETVAL, ((int *)ptrMemShared)[WIDTH * HEIGHT + 1]);
    }
  else
    {
      usleep(500);
    }
}

static int	checkMemory(key_t key, int *sem_id, int *msg_id, int memId)
{
  if (memId < 0)
    {
      perror("shmget");
      return (1);
    }
  if ((*sem_id = semget(key, NB_SEM, IPC_CREAT | SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  if ((*msg_id = msgget(key, IPC_CREAT | SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  return (0);
}

int	firstProcess(key_t key, int memId, int teamNb)
{
  int		sem_id;
  int		msg_id;
  t_player	player;
  bool		start;
  void		*ptrMemShared;

  if (checkMemory(key, &sem_id, &msg_id, memId))
    return (1);
  if ((ptrMemShared = shmat(memId, NULL, SHM_R | SHM_W)) == (void *) - 1)
    {
      perror("shmat");
      return (1);
    }
  initMemory(ptrMemShared, sem_id);
  putPlayerInMap(teamNb, ptrMemShared, &player, 1);
  start = false;
  while (!start || !isGameOver(ptrMemShared, sem_id))
    {
      testStartGame(&start, ptrMemShared, sem_id);
      doingStuff(sem_id, msg_id, ptrMemShared, &player);
    }
  return (prepareToExit(memId, sem_id, msg_id, ptrMemShared));
}
