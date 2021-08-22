#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
int main()
{
    int fd[2];
    assert(pipe(fd) != -1);
    //上面如果通过说明管道被创建

    //fd[0] r , fd[1] w
    //r端只能进行读，w同理
    pid_t pid = fork();
    assert(pid != -1);
    if (pid == 0)
    {
        close(fd[1]);
        while (1)
        {
            char buff[128] = {0};
            int n = read(fd[0], buff, 127);
            printf("子进程读取管道数据buff=%s\n", buff);
            if (n == 0)
            {
                break;
            }
        }
        close(fd[0]);
        printf("子进程结束,关闭管道写端\n");
        exit(0);
    }
    else
    {
        close(fd[0]);
        while (1)
        {
            char buff[128] = {0};
            fgets(buff, 128, stdin);
            if (strncmp(buff, "end", 3) == 0)
            {
                break;
            }
            write(fd[1], buff, strlen(buff));

            printf("父进程写入无名管道数据buff=%s\n", buff);
        }

        close(fd[1]);
        printf("父进程结束,关闭管道写端\n");
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    exit(0);
}