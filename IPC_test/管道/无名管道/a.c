#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<fcntl.h>
int main(){
    int fd[2];
    assert( pipe(fd) != -1 );
    //上面如果通过说明管道被创建
    
    //fd[0] r , fd[1] w
    //r端只能进行读，w同理
    write(fd[1],"hello",5);
    
    char buff[128]={0};
    read(fd[0],buff,127);
    printf("buff=%s\n",buff);
    close(fd[0]);
    close(fd[1]);
    
    exit(0);
    
}