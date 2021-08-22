#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<fcntl.h>
int main(){
    //管道只能够以只读或者只写方式打开
    // 打开创建的fifo有名管道
    // 注意这里采用系统命令和系统调用mkfifo来创建管道
    // mkfifo fifo
    // ll 会出现一个管道文件
    int fd = open("fifo",O_WRONLY);
    assert( fd != -1);
    
    char buff[128] = {0};
    printf("input:\n");
    fgets(buff,128,stdin);
    
    //数据写到管道里面
    write(fd,buff,strlen(buff));
    
    close(fd);
    
    exit(0);
    
}