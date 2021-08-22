#include "sem.h"
#define SEM_NUM 2

static int semid = -1;

int sem_init()
{
    semid = semget((key_t)1234, SEM_NUM, IPC_CREAT | IPC_EXCL | 0600);
    if (semid == -1)
    {
        semid = semget((key_t)1234, SEM_NUM, 0600);
        if (semid == -1)
        {
            perror("semget err");
            return -1;
        }
    }
    else
    {
        int a[SEM_NUM] = {1, 0};
        union semun semval;
        int i = 0;
        for (; i < SEM_NUM; ++i)
        {
            semval.val = a[i];
            if (semctl(semid, i, SETVAL, semval) == -1)
            {
                perror("semctl setval err");
                return -1;
            }
        }
    }
    return 0;
}
int sem_p(int index) //第几个信号量
{
    if (index < 0 || index >= SEM_NUM)
    {
        printf("sem_p : index arg error\n");
        return -1;
    }
    struct sembuf buf;
    buf.sem_num = index; //第几个信号量
    buf.sem_op = -1;     //操作
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1)
    {
        perror("semop p error");
        return -1;
    }

    return 0;
}
int sem_v(int index)
{
    if (index < 0 || index >= SEM_NUM)
    {
        printf("sem_v : index arg error\n");
        return -1;
    }
    struct sembuf buf;
    buf.sem_num = index; //第几个信号量
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