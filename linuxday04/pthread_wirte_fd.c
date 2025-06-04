#include "my_header.h"

void * threadfunc(void * arg){
    int *fd=(int *)arg;
    char *str="This is second process write1\n";
    write(*fd,str,strlen(str));
    close(*fd);
    return NULL;
}

int main(){
    pthread_t ptid;
    int fd=open("file1.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
    ERROR_CHECK(fd,-1,"open txt error\n");


    int ret=pthread_create(&ptid,NULL,threadfunc,&fd);
    THREAD_ERROR_CHECK(ret,"pthread create error\n");
    char *str="This is main process write!\n";
    write(fd,str,strlen(str));
    pthread_join(ptid,NULL);
    close(fd);
    return 0;
}
