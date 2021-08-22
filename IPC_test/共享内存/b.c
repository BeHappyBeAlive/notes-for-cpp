#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/shm.h>
#include "sem.h"
int main()
{
    int shmid = shmget((key_t)1234, 256, IPC_CREAT | 0600);
    //如果别人创建好了就直接获取该块的共享内存
    assert(shmid != -1);
    // 映射到共享内存中
    char *s = (char *)shmat(shmid, NULL, 0);
    //这里面的0表示既可读也可写
    assert(s != NULL);
    // 初始化信号量
    assert(sem_init() != -1);

    while (1)
    {
        //对第二个信号量p 使得其可以读 因为题目设置为只有一个输入，了一个才能输出
        sem_p(1);
        //判断消息队列中是否有end字符串，由此可以看出是b先断开共享内存
        if (strncmp(s, "end", 3) == 0)
        {
            break;
        }
        printf("s=%s\n", s);
        sem_v(0);
    }

    shmdt(s);
    //断开映射
    sem_destroy();
    exit(0);
}