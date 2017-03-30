/*
** lemipc.c for lemipc in /home/duhieu_b/Concurrence/PSU_2016_lemipc/src
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Mon Mar 20 10:51:43 2017 duhieu_b
** Last update Thu Mar 30 14:35:18 2017 duhieu_b
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

int	countPlayerInMap(void *ptrMemShared)
{
  int i;
  int res;
    int *map;

  i = 0;
  res = 0;
    map = ptrMemShared;
  while (i < WIDTH * HEIGHT)
    {
      if (map[i])
	res++;
      i++;
    }
  return (res);
}

void	putPlayerInMap(int teamNb, void *ptrMemShared, t_player *player, int turn)
{
  int pos;
    int *map;

    map = ptrMemShared;
    while ((pos = rand() % (WIDTH * HEIGHT)) >= 0 && map[pos]);
    map[pos] = teamNb;
    player->x = pos % WIDTH;
    player->y = pos / WIDTH;
    player->team = teamNb;
    player->turn = turn;
    player->dead = false;
}

bool	notGameOver(int *map)
{
  return (!map[WIDTH * HEIGHT]);
}

void goToGame(int teamNb, int sem_id, int msg_id, void *ptrMemShared, t_player *player)
{
  int		playerMax;
  struct sembuf sops[NB_SEM];

  /*  while (semctl(sem_id, GRAPH, GETVAL))
    {
      printf("WAITING GETVAL LOOP : %d\n", semctl(sem_id, LOOP, GETVAL));
      printf("WAITING GETVAL GRAPH : %d\n\n", semctl(sem_id, GRAPH, GETVAL));
      usleep(1);
      }*/
  /* sops[LOOP].sem_op = 1; */
  sops[LOOP].sem_num = LOOP;
  sops[LOOP].sem_flg = 0;
  sops[QUIT].sem_num = QUIT;
  sops[QUIT].sem_flg = 0;
  sops[QUIT].sem_op = -1;
  semop(sem_id, &sops[QUIT], 1);
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] +=1;
  sops[QUIT].sem_op = 1;
  semop(sem_id, &sops[QUIT], 1);
  playerMax = countPlayerInMap(ptrMemShared) + 1;
  semctl(sem_id, LOOP, SETVAL, playerMax);
  putPlayerInMap(teamNb, ptrMemShared, player, playerMax);
  printf("BEFORE THE LOOP GETVAL LOOP : %d\n", semctl(sem_id, LOOP, GETVAL));
  sops[LOOP].sem_op = -1;
  sops[GRAPH].sem_num = GRAPH;
  sops[GRAPH].sem_flg = 0;
  sops[OVER].sem_num = OVER;
  sops[OVER].sem_flg = 0;
  sops[OVER].sem_op = 0;
  while (notGameOver(ptrMemShared))
    {
      semop(sem_id, &sops[OVER], 1);
      if (!player->dead && semctl(sem_id, LOOP, GETVAL) == player->turn)
	{
	  if (checkDead(player, ptrMemShared))
	    {
	      ((int *)ptrMemShared)[player->x + player->y * WIDTH] = 0;
	      player->dead = true;
	      semop(sem_id, &sops[LOOP], 1);
	      semop(sem_id, &sops[GRAPH], 1);
	    }
	  moveAtRandom(msg_id, player, ptrMemShared);
	  semop(sem_id, &sops[LOOP], 1);
	  sops[GRAPH].sem_op = 1;
	  semop(sem_id, &sops[GRAPH], 1);
	  usleep(10);
	  sops[GRAPH].sem_op = -1;
	  semop(sem_id, &sops[GRAPH], 1);
	}
      else
	{
	  if (player->dead)
	    semop(sem_id, &sops[LOOP], 1);
	  usleep(10);
	}
    }
  sops[QUIT].sem_op = -1;
  semop(sem_id, &sops[QUIT], 1);
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] -=1;
  sops[QUIT].sem_op = 1;
  semop(sem_id, &sops[QUIT], 1);
}

bool	isTeams(void *ptrMemShared)
{
  int	comp;
  int	i;
    int *map;

  comp = -1;
    map = ptrMemShared;
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      if (comp == -1 && map[i])
	comp = map[i];
      else if (map[i] && comp != map[i])
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
    printf(" ");
  while (i < WIDTH)
    {
      printf(" %d", i);
      i++;
    }
  printf("\n ");
  i = 0;
  while (i < WIDTH)
    {
      printf("__");
      i++;
    }
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      if (!(i % WIDTH))
	printf("\n%d|", i / WIDTH);
      if (ptrMemShared[i])
	printf(" %c", ptrMemShared[i] + 65);
      else
	printf("  ");
      i++;
    }
    printf("\n");
}

int		shared_memory(key_t key, int teamNb)
{
  int		memId;
  int		sem_id;
  int		msg_id;
  t_player	player;
  //  struct sembuf sops[2];
  bool		start;
  void		*ptrMemShared;

  if ((memId = shmget(key, (HEIGHT * WIDTH + 2) * sizeof(int),
		      0666 | IPC_CREAT | IPC_EXCL)) == -1)
    {
      printf("Entered\n");
      if ((memId = shmget(key, (HEIGHT * WIDTH + 2) * sizeof(int), 0444)) < 0)
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
      if ((msg_id = msgget(key, SHM_R | SHM_W)) < 0)
	{
	  perror("semget");
	  return (1);
	}
      printf("Go to game\n");
      goToGame(teamNb, sem_id, msg_id, ptrMemShared, &player);
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
  if ((msg_id = msgget(key, IPC_CREAT | SHM_R | SHM_W)) < 0)
    {
      perror("semget");
      return (1);
    }
  memset(ptrMemShared, 0, (WIDTH * HEIGHT + 2) * sizeof(int));
  ((int *)ptrMemShared)[WIDTH * HEIGHT + 1] = 1;
  start = false;
  semctl(sem_id, LOOP, SETVAL, 1);
  semctl(sem_id, GRAPH, SETVAL, 1);
  semctl(sem_id, QUIT, SETVAL, 1);
  putPlayerInMap(teamNb, ptrMemShared, &player, 1);
  while (!start || !isGameOver(ptrMemShared, sem_id))
    {
      if (!start && isTeams(ptrMemShared))
	start = true;
      if (semctl(sem_id, GRAPH, GETVAL) == 1)
        {
	  displayMap(ptrMemShared);
	  usleep(300000);
        }
      if (semctl(sem_id, LOOP, GETVAL) == 1)
        {
	  if (!player.dead)
	    {
	      if (!checkDead(&player, ptrMemShared))
		moveAtRandom(msg_id, &player, ptrMemShared);
	      else
		{
		  ((int *)ptrMemShared)[player.x + player.y * WIDTH] = 0;
		  player.dead = true;
		}
	      displayMap(ptrMemShared);
	      usleep(300000);
	    }
	  semctl(sem_id, LOOP, SETVAL, countPlayerInMap(ptrMemShared));
        }
      else
        {
	  usleep(10);
        }
    }
  //  printf("OVER\n");
  //displayMap(ptrMemShared);
  while (((int *)ptrMemShared)[WIDTH * HEIGHT + 1] > 1);
  shmctl(memId, IPC_RMID, NULL);
  semctl(sem_id, LOOP, IPC_RMID);
  msgctl(msg_id, IPC_RMID, NULL);
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
