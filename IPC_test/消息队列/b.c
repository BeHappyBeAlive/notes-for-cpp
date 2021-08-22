#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/msg.h>

struct mess
{
    long int type;
    char buff[32];
};

int main(){
    int msgid = msgget((key_t)1234,IPC_CREAT|0600);
    assert( msgid != -1 );
    
    struct mess dt;
    msgrcv(msgid,&dt,32,1,0);
    //msgrcv(msgid,&dt,32,1,0);
    //这里面的参数中的第四个如果为0 则表示不管是什么类型都可以接受
    //消息队列id 消息结构体 大小 类型 标志 
    //读取消息队列中的消息
    printf("read msg:%s\n",dt.buff);
    
    exit(0);
    
}