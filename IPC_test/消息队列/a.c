#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/msg.h>

struct mess
{
    long int type;
    char mtext[32];
};

int main(){
    int msgid = msgget((key_t)1234,IPC_CREAT|0600);
    assert( msgid != -1 );
    
    struct mess data;
    data.type = 1;
    strcpy(data.mtext,"hello1");
    
    msgsnd(msgid,&data,32,0);
    //往消息队列中写入一个数据
    
    exit(0);
    
}