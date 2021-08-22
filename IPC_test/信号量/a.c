#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include "sem.h"

int main(){
    sem_init();
    int i = 0;
    for(;i<5;i++){
        sem_p();
        write(1,"A",1);
        int n = rand()%3;
        sleep(n);
        write(1,"A",1);
        sem_v();
        n = rand()%3;
        sleep(n);
    }
    sleep(10);
    sem_destroy();
    exit(0);
    
    
}