#include <65func.h>
typedef struct shareRes_s {
    int ticketNum;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;
void *sellTicket1(void *arg){
    shareRes_t * pshareRes = (shareRes_t *)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->ticketNum <= 0){
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("Before sell window1, ticketNum = %d\n", pshareRes->ticketNum);
        --pshareRes->ticketNum;
        printf("After sell window1, ticketNum = %d\n", pshareRes->ticketNum);
        if(pshareRes->ticketNum <10){
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
        if(pshareRes->ticketNum <= 0){
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("Before sell window2, ticketNum = %d\n", pshareRes->ticketNum);
        --pshareRes->ticketNum;
        printf("After sell window2, ticketNum = %d\n", pshareRes->ticketNum);
        if(pshareRes->ticketNum <10){
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
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);

    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,sellTicket1,&shareRes);
    pthread_create(&tid2,NULL,sellTicket2,&shareRes);
    pthread_create(&tid3,NULL,addTicket,&shareRes);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    return 0;
}

