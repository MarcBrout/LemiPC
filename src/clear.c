//
// Created by brout_m on 30/03/17.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stddef.h>
#include <string.h>
#include "game.h"

int main(int ac, char **av)
{
    key_t key;
    int mem;
    int *tab;

    key = ftok(av[1], 12345);
    mem = shmget(key, (WIDTH * HEIGHT + 2) * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL);
    tab = shmat(mem, NULL, SHM_R | SHM_W);
    bzero(tab, (WIDTH * HEIGHT + 2) * sizeof(int));
    return (0);
}