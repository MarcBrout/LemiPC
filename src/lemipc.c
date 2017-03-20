/*
** lemipc.c for lemipc in /home/duhieu_b/Concurrence/PSU_2016_lemipc/src
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Mon Mar 20 10:51:43 2017 duhieu_b
** Last update Mon Mar 20 13:01:57 2017 duhieu_b
*/

#include <sys/types.h>
#include <sys/ips.h>
#include <sys/shm.h>
#include "lemipc.h"

int	shared_memory(key_t key, int teamNB)
{
  int memId;
  void *ptrMemShared;

  if ((memId = shmget(CLEF, HEIGHT * WIDTH * sizeof(struct s_case),
		      0666 | IPC_CREAT | IPC_EXCL)) == EERNO)
    {
      if ((memId = shmget(CLEF, HEIGHT * WIDTH * sizeof(struct s_case), 0444)) < 0)
	{
	  perror("shmget");
	  return (1);
	}
    }
  if (memID < 0)
    {
      perror("shmget");
      return (1);
    }
  if ((ptrMemShared = shmat(memId, NULL, 0)) == (void *) - 1)
    {
      perror("shmat");
      return (1);
    }
  return (0);
}

int	lemipc(char *path, int teamNb)
{
  key_t key;

  if (teamNB <= 0)
    {
      fprintf(2, "Error: teamNB must be greater than 0.\n");
      return (1);
    }
  key = ftok(path, 12345);
  shared_memory(key, teamNB);
  return (0);
}

int	main(int ac, char **av)
{
  if (ac != 3 || !av)
    {
      fprintf(2, "Usage: ./lemipc PATH TEAM_NUMBER\n");
      return (1);
    }
  if (lemipc(av[1], atoi(av[2])))
    return (1);
  return (0);
}
