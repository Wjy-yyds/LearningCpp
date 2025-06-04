#include "my_header.h"
typedef struct shareRes_s{
    int ticket_count;
    pthread_mutex_t mutex;
} shareRes_t;

pthread_t ptid1, ptid2;

//标准错误，while在使用ticket时也要考虑race condition
// void * sale1(void * arg){
//     shareRes_t *shareRes=(shareRes_t *)arg;
    
//     while(shareRes->ticket_count>0){
//         pthread_mutex_lock(&shareRes->mutex);
//         printf("sale1 now have: %d tickets ,and now sold one!\n",shareRes->ticket_count);
//         shareRes->ticket_count--;
        
//         pthread_mutex_unlock(&shareRes->mutex);
//         sleep(1);
//     }
   
//     pthread_exit(NULL);
// }

// void * sale2(void * arg){
//     shareRes_t *shareRes=(shareRes_t *)arg;

//     while(shareRes->ticket_count>0){
//         pthread_mutex_lock(&shareRes->mutex);
//         printf("sale2 now have: %d tickets ,and now sold one!\n",shareRes->ticket_count);
//         shareRes->ticket_count--;
      
//         pthread_mutex_unlock(&shareRes->mutex);
//         sleep(1);
//     }

//     pthread_exit(NULL);
// }

void *sale_ticket(void *arg){
    shareRes_t *shareRes=(shareRes_t *)arg;
    char *name=(pthread_self()==)
}


int main(){
    
    
    shareRes_t shareRes;
    shareRes.ticket_count=20;
    pthread_mutex_init(&shareRes.mutex,NULL);

    pthread_t ptid1,ptid2;
    struct timeval begTime,endTime;
    gettimeofday(&begTime,NULL);
    pthread_create(&ptid1,NULL,sale1,&shareRes);
    pthread_create(&ptid2,NULL,sale2,&shareRes);
    



    pthread_join(ptid1,NULL);
    pthread_join(ptid2,NULL);
    gettimeofday(&endTime,NULL);
    printf("Total time = %ld\n",endTime.tv_sec-begTime.tv_sec);

    
    return 0;
}