#include "my_header.h"
typedef struct ListNode{
    int data;
    struct ListNode *next;
}ListNode;

typedef struct shareRes_s{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    ListNode *head;
    ListNode *tail;
}shareRes_t;

void * threadfunc(void * arg){


    shareRes_t *shareRes=(shareRes_t *)arg;


    pthread_mutex_lock(&shareRes->mutex);
    if(shareRes->flag!=1){
        pthread_cond_wait(&shareRes->cond,&shareRes->mutex);
    }
    pthread_mutex_unlock(&shareRes->mutex);

   printf(" B is done\n");

    pthread_mutex_lock(&shareRes->mutex);
    shareRes->flag=2;
    pthread_cond_signal(&shareRes->cond);
    pthread_mutex_unlock(&shareRes->mutex);
    

    pthread_exit(NULL);
}

int main(){
    pthread_t ptid;

    shareRes_t shareRes;
    shareRes.flag=0;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);
    
    int ret=pthread_create(&ptid,NULL,threadfunc,&shareRes);
    THREAD_ERROR_CHECK(ret,"pthread create error\n");


  
     printf("A  is done!\n");
    //创建链表后才允许
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.flag=1;
    pthread_cond_signal(&shareRes.cond);
    pthread_mutex_unlock(&shareRes.mutex);
 


    //pthread_join(ptid,NULL);
    pthread_mutex_lock(&shareRes.mutex);
    if(shareRes.flag!=2){
        pthread_cond_wait(&shareRes.cond,&shareRes.mutex);
    }
    pthread_mutex_unlock(&shareRes.mutex);

    printf("C is done\n");
    
    return 0;
}
