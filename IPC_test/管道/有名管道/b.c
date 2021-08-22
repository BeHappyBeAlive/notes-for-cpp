#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<fcntl.h>
int main(){
    //管道只能够以只读或者只写方式打开
    int fd = open("fifo",O_RDONLY);
    assert( fd != -1);
    char buff[128] = {0};
    // 该方法是阻塞方法，如果管道的写端没有向管道中写数据，那么该方法会阻塞住
    // 一旦管道写端关闭，则读操作会被阻塞
    int n = read(fd,buff,128);
    //这个时候管道没有读取到数据
    //就会在此阻塞住
    printf("read:%s\n",buff);
   
    
    close(fd);
    
    exit(0);
    
}

