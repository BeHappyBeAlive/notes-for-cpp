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
        write(1,"B",1);
        int n = rand()%3;
        sleep(n);
        write(1,"B",1);
        sem_v();
        n = rand()%3;
        sleep(n);
    }   
    exit(0);    
}