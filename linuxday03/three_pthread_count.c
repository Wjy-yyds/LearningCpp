#include <my_header.h>
#define NUM 10000000


/* Usage:  */
void * threadFunc(void *arg){
    long *k=(long *)arg;
    for(int j=0;j<NUM;j++){
        (*k)++;
    }

    return NULL;
}


int main(int argc, char *argv[]){                                  
   
    //创建一个线程
    pthread_t tid;
    long *i=(long *)malloc(sizeof(long));
    *i=0;
    int ret=pthread_create(&tid,NULL,threadFunc,i);
    THREAD_ERROR_CHECK(ret,"pthread create failed\n");
    for(int j=0;j<NUM;j++){
        (*i)++;
    }
    
    pthread_join(tid,NULL);
    printf("All is over,the final i is %ld \n",*i);
    free(i);
    

    return 0;
    
}

