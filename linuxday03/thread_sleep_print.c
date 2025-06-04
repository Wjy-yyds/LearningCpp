#include "my_header.h"

void *threadFunc(void *arg){
    printf("I am child thread!\n");
    return NULL; // (void *)0
}
int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_create(&tid,NULL,threadFunc,NULL);
    printf("I am main thread!\n");
    usleep(2);
    return 0;  }
