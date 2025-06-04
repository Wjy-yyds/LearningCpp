#include "my_header.h"

typedef struct shareRes_s {
    int ticketNum; 
    pthread_mutex_t mutex; 
    pthread_cond_t cond; 
} shareRes_t;

void *sellTicket1(void *arg){
    shareRes_t * pshareRes = (shareRes_t *)arg;

    while(1){
        pthread_mutex_lock(&pshareRes->mutex);

        if(pshareRes->ticketNum<=0){
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("sale1 begin sale!\n");
        --pshareRes->ticketNum;
        printf("after sale1 the num is %d\n",pshareRes->ticketNum);
        if(pshareRes->ticketNum<10){
            pthread_cond_signal(&pshareRes->cond);
        }
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(1);
    }
    return NULL;
}

void *sellTicket2(void *arg){
    shareRes_t * pshareRes = (shareRes_t *)arg;

    while(1){
        pthread_mutex_lock(&pshareRes->mutex);

        if(pshareRes->ticketNum<=0){
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("sale2 begin sale!\n");
        --pshareRes->ticketNum;
        printf("after sale2 the num is %d\n",pshareRes->ticketNum);
        if(pshareRes->ticketNum<10){
            pthread_cond_signal(&pshareRes->cond);
        }
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(1);
    }
    return NULL;
}

void * addTicket(void *arg){
    shareRes_t * pshareRes = (shareRes_t *)arg;
    pthread_mutex_lock(&pshareRes->mutex);
    if(pshareRes->ticketNum > 10){
        printf("ticket is enough now!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    pshareRes->ticketNum += 10; //后事件如果要访问共享资源，可以推迟一会解锁
    printf("add 10 tickets! ticketNum = %d\n", pshareRes->ticketNum);
    pthread_mutex_unlock(&pshareRes->mutex);
    return NULL;
}

int main(int argc, char *argv[])
{
    shareRes_t shareRes;
    shareRes.ticketNum = 20;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    pthread_t ptid1, ptid2, ptid3;

    pthread_create(&ptid1, NULL, sellTicket1, &shareRes);
    pthread_create(&ptid2, NULL, sellTicket2, &shareRes);
    pthread_create(&ptid3, NULL, addTicket, &shareRes);

    pthread_join(ptid1, NULL);
    pthread_join(ptid2, NULL);
    pthread_join(ptid3, NULL);

    printf("All tickets sold!\n");


    return 0;
}
