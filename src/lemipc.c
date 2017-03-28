/*
** lemipc.c for lemipc in /home/duhieu_b/Concurrence/PSU_2016_lemipc/src
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Mon Mar 20 10:51:43 2017 duhieu_b
** Last update Tue Mar 28 11:11:54 2017 duhieu_b
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "lemipc.h"
#define HEIGHT 1
#define WIDTH 1

int		shared_memory(key_t key, int teamNB)
{
  int		memId;
  int		sem_id;
  int		comp;
  int		loop;
  struct sembuf sops;
  void		*ptrMemShared;

  if ((memId = shmget(key, HEIGHT * WIDTH * sizeof(struct s_case),
		      0666 | IPC_CREAT | IPC_EXCL)) == -1)
    {
      if ((memId = shmget(key, HEIGHT * WIDTH * sizeof(struct s_case), 0444)) < 0)
	{
	  perror("shmget");
	  return (1);
	}
      if ((sem_id = semget(key, 1, SHM_R | SHM_W)) < 0)
	{
	  perror("semget");
	  return (1);
	}
      if ((ptrMemShared = shmat(memId, NULL, SHM_R | SHM_W)) == (void *) - 1)
	{
	  perror("shmat");
	  return (1);
	}
      sops.sem_num = 0;
      sops.sem_flg = 0;
      sops.sem_op = -1;
      printf("SEM BEFORE: %d\n", semctl(sem_id, 0, GETVAL));
      semop(sem_id, &sops, 1);
      printf("SEM AFTER: %d\n", semctl(sem_id, 0, GETVAL));
      ((struct s_case *)ptrMemShared)->nteam = teamNB;
      printf("POS X : %d\n", ((struct s_case *)ptrMemShared)->x);
      printf("POS Y : %d\n", ((struct s_case *)ptrMemShared)->y);
      return (0);
    }
  printf("Creating SHARED_MEMORY: %d\n", memId);
  /* printf("EEXIST : %d\n", EEXIST); */
  if (memId < 0)
    {
      perror("shmget");
      return (1);
    }
  if ((ptrMemShared = shmat(memId, NULL, SHM_R | SHM_W)) == (void *) - 1)
    {
      perror("shmat");
      return (1);
    }
  if ((sem_id = semget(key, NB_SEM, IPC_CREAT | SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  printf("Creating New Semaphore\n");
  semctl(sem_id, LOOP, SETVAL, 4);
  printf("Setting Semaphore value to : %d\n", semctl(sem_id, 0, GETVAL));
  ((struct s_case *)ptrMemShared)->x = 0;
  ((struct s_case *)ptrMemShared)->y = 1;
  ((struct s_case *)ptrMemShared)->nteam = teamNB;
  comp = semctl(sem_id, 0, GETVAL);
  printf("POS X : %d\n",   ((struct s_case *)ptrMemShared)->x);
  printf("POS Y : %d\n",   ((struct s_case *)ptrMemShared)->y);
  printf("NTEAM : %d\n", ((struct s_case *)ptrMemShared)->nteam);
  while ((loop = semctl(sem_id, 0, GETVAL)) > 0)
    {
      if (loop < comp)
	{
	  comp = loop;
	  ((struct s_case *)ptrMemShared)->x += 1;
	  ((struct s_case *)ptrMemShared)->y += 1;
	  usleep(10);
	  printf("NTEAM : %d\n", ((struct s_case *)ptrMemShared)->nteam);
	}
    }
  shmctl(memId, IPC_RMID, NULL);
  semctl(sem_id, 0, IPC_RMID);
  return (0);
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

int	main(int ac, char **av)
{
  if (ac != 3 || !av)
    {
      dprintf(2, "Usage: ./lemipc PATH TEAM_NUMBER\n");
      return (1);
    }
  if (lemipc(av[1], atoi(av[2])))
    return (1);
  return (0);
}
