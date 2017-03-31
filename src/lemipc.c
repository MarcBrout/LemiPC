/*
** lemipc.c for lemipc in /home/duhieu_b/Concurrence/PSU_2016_lemipc/src
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Mon Mar 20 10:51:43 2017 duhieu_b
** Last update Thu Mar 30 18:23:27 2017 duhieu_b
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

bool graphic = false;

bool	notGameOver(int *map)
{
  return (!map[WIDTH * HEIGHT]);
}

int prepareToExit(int memId, int sem_id, int msg_id, void *ptrMemShared)
{
    if (graphic)
  displayMap(ptrMemShared);
  while (((int *)ptrMemShared)[WIDTH * HEIGHT + 1] > 1);
  shmctl(memId, IPC_RMID, NULL);
  semctl(sem_id, LOOP, IPC_RMID);
  msgctl(msg_id, IPC_RMID, NULL);
  return (0);
}

int		shared_memory(key_t key, int teamNb)
{
  int		memId;

  if ((memId = shmget(key, (HEIGHT * WIDTH + 2) * sizeof(int),
		      0666 | IPC_CREAT | IPC_EXCL)) == -1)
    {
      return (otherProcess(key, memId, teamNb));
    }
  return (firstProcess(key, memId, teamNb));
}

int	lemipc(char *path, int teamNb)
{
  key_t key;

  if (teamNb <= 0)
    {
      dprintf(2, "Error: teamNB must be greater than 0.\n");
      return (1);
    }
  key = ftok(path, 12345);
  shared_memory(key, teamNb);
  return (0);
}

int	main(int ac, char **    av)
{
  srand(time(0));
  if (ac < 3 || !av)
    {
      dprintf(2, "Usage: ./lemipc PATH TEAM_NUMBER\n");
      return (1);
    }
    if (ac == 4 && (!strcmp(av[3], "-g") || !strcmp(av[3], "--graphic")))
        graphic = true;
  if (lemipc(av[1], atoi(av[2])))
    return (1);
  return (0);
}
