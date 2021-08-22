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
    // 映射到一块共享内存空间，通过shmid
    char *s = (char *)shmat(shmid, NULL, 0);
    //这里面的0表示既可读也可写
    assert(s != NULL);
    // 初始化信号量
    assert(sem_init() != -1);
    while (1)
    {
        printf("input:\n");
        char buff[128] = {0};
        //通过标准输入流输入一个字串存到buff中
        fgets(buff, 128, stdin);
        // 对第1个信号量进行p操作 其初始值为1
        sem_p(0);
        // 将buff拷贝到s共享内存中
        strcpy(s, buff);
        // 第2个信号量执行v操作 其初始值为0
        sem_v(1);
        // 如果输入end则结束
        if (strncmp(buff, "end", 3) == 0)
        {
            break;
        }
        //由此可以看出是b先退出
        // 所以对于信号量的销毁是在b中进行
    }
    shmdt(s);
    //断开映射

    exit(0);
}