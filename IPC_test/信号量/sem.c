#include "sem.h"

static int semid = -1;

int sem_init()
{
    semid = semget((key_t)1234, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (semid == -1)
    {
        semid = semget((key_t)1234, 1, 0600);
        if (semid == -1)
        {
            perror("semget err");
            return -1;
        }
    }
    else
    {
        union semun semval;
        semval.val = 1;
        if (semctl(semid, 0, SETVAL, semval) == -1)
        {
            perror("semctl setval err");
            return -1;
        }
    }

    return 0;
}
int sem_p() //第几个信号量
{
    
    struct sembuf buf;
    buf.sem_num = 0; //第几个信号量
    buf.sem_op = -1;     //操作
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1)
    {
        perror("semop p error");
        return -1;
    }

    return 0;
}
int sem_v()
{
    
    struct sembuf buf;
    buf.sem_num = 0; //第几个信号量
    buf.sem_op = 1;      //操作
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1)
    {
        perror("semop v error");
        return -1;
    }

    return 0;
}

void sem_destroy()
{
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        //0表示占位置的作用
        perror("semctl rm error");
    }
}