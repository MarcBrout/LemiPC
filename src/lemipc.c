/*
** lemipc.c for lemipc in /home/duhieu_b/Concurrence/PSU_2016_lemipc/src
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Mon Mar 20 10:51:43 2017 duhieu_b
** Last update Tue Mar 28 16:33:30 2017 duhieu_b
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "lemipc.h"

int	countPlayerInMap(void *ptrMemShared)
{
  int i;
  int res;

  i = 0;
  res = 0;
  while (i < WIDTH * HEIGHT)
    {
      if (((int *)ptrMemShared)[i])
	res++;
      i++;
    }
  return (res);
}

void	putPlayerInMap(int teamNb, void *ptrMemShared, t_player *player, int turn)
{
  int pos;

  pos = rand() % (WIDTH * HEIGHT);
  if (((int *)ptrMemShared)[pos])
    {
      ((int *)ptrMemShared)[pos] = teamNb;
      player->x = pos % WIDTH;
      player->y = pos / HEIGHT;
      player->team = teamNb;
      player->turn = turn;
    }
  else
    {
      while ((pos = rand() % (WIDTH * HEIGHT)) != 0);
      ((int *)ptrMemShared)[pos] = teamNb;
      player->x = pos % WIDTH;
      player->y = pos / HEIGHT;
      player->team = teamNb;
      player->turn = turn;
    }
}

void goToGame(int teamNb, int sem_id, void *ptrMemShared, t_player *player)
{
  int		playerMax;
  struct sembuf sops[2];

  playerMax = countPlayerInMap(ptrMemShared) + 1;
  semctl(sem_id, LOOP, SETVAL, playerMax);
  while (semctl(sem_id, GRAPH, GETVAL))
    usleep(1);
  putPlayerInMap(teamNb, ptrMemShared, player, playerMax);
  sops[LOOP].sem_num = 0;
  sops[LOOP].sem_flg = 0;
  sops[LOOP].sem_op = -1;
  sops[GRAPH].sem_num = 0;
  sops[GRAPH].sem_flg = 0;
  sops[GRAPH].sem_op = 1;
  while (42)
    {
      if (semctl(sem_id, LOOP, GETVAL) == player->turn)
	{
	  if (checkDead(player, ptrMemShared))
	    {
	      ((int *)ptrMemShared)[x + y * WIDTH] = 0;
	      semop(sem_id, &sops[LOOP], LOOP);
	      semop(sem_id, &sops[GRAPH], GRAPH);
	      break;
	    }
	  moveAtRandom(player, ptrMemShared);
	  semop(sem_id, &sops[LOOP], LOOP);
	  semop(sem_id, &sops[GRAPH], GRAPH);
	}
      else
	usleep(10);
    }
}

bool	isTeams(void *ptrMemShared)
{
  int	comp;
  int	i;

  comp = -1;
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      if (comp == -1 && ((int *)ptrMemShared)[i])
	comp = ((int *)ptrMemShared)[i];
      else if (((int *)ptrMemShared)[i] && comp != ((int *)ptrMemShared)[i])
	return (true);
      i++;
    }
  return (false);
}

void		displayMap(int *ptrMemShared)
{
  int i;

  printf("\e[1;1H\e[2J");
  i = 0;
  while (i < WIDTH)
    {
      printf("%d", i);
      i++;
    }
  printf("\n");
  i = 0;
  while (i < WIDTH)
    {
      printf("_");
      i++;
    }
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      if (!(i % WIDTH))
	printf("\n%d|", i / HEIGHT);
      if (ptrMemShared[i])
	printf("%c", ptrMemShared[i] + 65);
      else
	printf(" ");
      i++;
    }
}

int		shared_memory(key_t key, int teamNb)
{
  int		memId;
  int		sem_id;
  t_player	player;
  struct sembuf sops[2];
  bool		start;
  void		*ptrMemShared;

  if ((memId = shmget(key, HEIGHT * WIDTH * sizeof(int),
		      0666 | IPC_CREAT | IPC_EXCL)) == -1)
    {
      if ((memId = shmget(key, HEIGHT * WIDTH * sizeof(int), 0444)) < 0)
	{
	  perror("shmget");
	  return (1);
	}
      if ((sem_id = semget(key, NB_SEM, SHM_R | SHM_W)) < 0)
	{
	  perror("semget");
	  return (1);
	}
      if ((ptrMemShared = shmat(memId, NULL, SHM_R | SHM_W)) == (void *) - 1)
	{
	  perror("shmat");
	  return (1);
	}
      goToGame(teamNb, sem_id, ptrMemShared, &player);
      return (0);
    }
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
  start = false;
  semctl(sem_id, LOOP, SETVAL, 1);
  semctl(sem_id, GRAPH, SETVAL, 1);
  putPlayerInMap(teamNb, ptrMemShared, player, 1);
  sops[GRAPH].sem_num = 0;
  sops[GRAPH].sem_flg = 0;
  sops[GRAPH].sem_op = -1;
  while (!isGameOver() || !start)
    {
      if (!start && isTeams(ptrMemShared))
	start = true;
      if (semctl(sem_id, GRAPH, GETVAL))
	{
	  semop(sem_id, &sem_id[GRAPH], GRAPH);
	  displayMap(ptrMemShared);
	}
      if (!semctl(sem_id, LOOP, GETVAL))
	{
	  if (!checkDead(&player, ptrMemShared))
	    moveAtRandom(&player, ptrMemShared);
	  ((int *)ptrMemShared)[x + y * WIDTH] = 0;
	  sops[LOOP].sem_num = 0;
	  sops[LOOP].sem_flg = 0;
	  sops[LOOP].sem_op = countPlayerInMap(ptrMemShared);
	  semop(sem_id, &sem_id[LOOP], LOOP);
	}
    }
  shmctl(memId, IPC_RMID, NULL);
  semctl(sem_id, LOOP, IPC_RMID);
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
  srand(time(0));
  if (ac != 3 || !av)
    {
      dprintf(2, "Usage: ./lemipc PATH TEAM_NUMBER\n");
      return (1);
    }
  if (lemipc(av[1], atoi(av[2])))
    return (1);
  return (0);
}
