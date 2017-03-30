/*
** map.c for map in /home/duhieu_b/Concurrence/PSU_2016_lemipc
**
** Made by duhieu_b
** Login   <benjamin.duhieu@epitech.eu>
**
** Started on  Thu Mar 30 17:49:57 2017 duhieu_b
** Last update Thu Mar 30 18:18:06 2017 duhieu_b
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
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

void	displayMapNext(int *ptrMemShared)
{
  int i;

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

void	displayMap(int *ptrMemShared)
{
  int	i;

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
  displayMapNext(ptrMemShared);
}
