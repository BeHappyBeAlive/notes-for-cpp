#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/shm.h>
#include <sys/sem.h>
union semun
{
    int val;
};
int sem_init();

int sem_p(int index);

int sem_v(int index);

void sem_destroy();
