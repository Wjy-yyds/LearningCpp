#include <my_header.h>
#define NUM 10000000
typedef struct shareRes_s{
    int num;
    pthread_mutex_t mutex;
}shareRes_t;

void * pthreadadd(void *arg){
    shareRes_t * shareRes=(shareRes_t *)arg;
    for(int i=0;i<NUM;i++){
        pthread_mutex_lock(&shareRes->mutex);
        ++shareRes->num;
        pthread_mutex_unlock(&shareRes->mutex);
    } 

    pthread_exit(NULL);
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    shareRes_t shareRes;
    shareRes.num=0;
    pthread_t pthread_id;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_create(&pthread_id,NULL,pthreadadd,&shareRes);
    struct timeval begTime,endTime;
    gettimeofday(&begTime,NULL);
    for(int i=0;i<NUM;i++){
        pthread_mutex_lock(&shareRes.mutex);
        ++shareRes.num;
        pthread_mutex_unlock(&shareRes.mutex);
    }


    pthread_join(pthread_id,NULL);
    gettimeofday(&endTime,NULL);
    printf("Total time = %ld\n",endTime.tv_usec-begTime.tv_usec+1000000*(endTime.tv_sec-begTime.tv_sec));

    return 0;
}

