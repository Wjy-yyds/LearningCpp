#include "my_header.h"
#define MAXSIZE 10
#define MINSIZE 0

typedef struct ListNode_s {
    int data; //数据
    struct ListNode_s *next; //下一个节点
} ListNode_t;

typedef struct shareRes_s {
    int size; 
    ListNode_t *head; 
    ListNode_t *tail; 
    pthread_mutex_t mutex; 
    pthread_cond_t cond; 
} shareRes_t;

int initList(shareRes_t *pshareRes){

    pshareRes->size = 0;
    pshareRes->head = NULL;
    pshareRes->tail = NULL;
    
    return 0;
}

int  headDelete(shareRes_t *pshareRes){

    ListNode_t *node=pshareRes->head;
    int dtdata=pshareRes->head->data;
    pshareRes->head=node->next;

    if(pshareRes->size==1){
        pshareRes->tail=NULL;
    }
    free(node);
    --pshareRes->size;

    return dtdata;

}

int getnsrandom(){
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    unsigned long long seed=ts.tv_sec *1000000000ULL + ts.tv_nsec;
    srand(seed);
    return rand()%10+1;
}

int insertList(shareRes_t *pshareRes){

    ListNode_t *node=(ListNode_t *)calloc(1,sizeof(ListNode_t));

    
    int data=getnsrandom();

    node->data=data;
    node->next=NULL;

    if(pshareRes->size==0){
        pshareRes->head=node;
        pshareRes->tail=node;
    }else{
        pshareRes->tail->next=node;
        pshareRes->tail=node;
    }
    pshareRes->size++;

    return data;
}

int printList(shareRes_t *shareRes){
    ListNode_t *pCur=shareRes->head;
    while(pCur){
        printf(" %2d ",pCur->data);
        pCur=pCur->next;
    }
    printf("\n");
    return 0;
}

void * producethread1(void *arg){
    
    shareRes_t *pshareRes=(shareRes_t *)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
        while(pshareRes->size>=MAXSIZE){
            printf("producethread1 find goods now is full!\n");
            pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
            printf("producethread1 is wake up now!\n");
    }
    //现在可以继续生产了
    //++pshareRes->size;

    int pru=insertList(pshareRes);
    pthread_cond_broadcast(&pshareRes->cond);
    printf("producethread1 procdue %d,now goods num is %d\n",pru,pshareRes->size);
    pthread_mutex_unlock(&pshareRes->mutex);
    sleep(3);
    }
    
    return NULL;
}

void * producethread2(void *arg){
    shareRes_t *pshareRes=(shareRes_t *)arg;
    while(1){
         pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->size>=MAXSIZE){
        printf("producethread2 find goods now is full!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        printf("producethread2 is wake up now!\n");
    }
    //现在可以继续生产了
    //++pshareRes->size;
   int pru= insertList(pshareRes);
    pthread_cond_broadcast(&pshareRes->cond);
    printf("producethread2 procdue %d,now goods num is %d\n",pru,pshareRes->size);
    pthread_mutex_unlock(&pshareRes->mutex);
    sleep(3);
    }
   
    return NULL;
}

void * producethread3(void *arg){
    shareRes_t *pshareRes=(shareRes_t *)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->size>=MAXSIZE){
        printf("producethread3 find goods now is full!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        printf("producethread3 is wake up now!\n");
    }
    //现在可以继续生产了
    //++pshareRes->size;
    int pru=insertList(pshareRes);
    pthread_cond_broadcast(&pshareRes->cond);
    printf("producethread3 procdue %d,now goods num is %d\n",pru,pshareRes->size);
    pthread_mutex_unlock(&pshareRes->mutex);
    sleep(3);
    }
    
    return NULL;
}

void * consumerthread1(void *arg){
    shareRes_t *pshareRes=(shareRes_t *)arg;
    while(1){
         pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->size<=MINSIZE){
        printf("consumer1 find goods now is not enough!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        printf("consumer1 is wake up now!\n");
    }
    //--pshareRes->size;
    int dtn=headDelete(pshareRes);
    pthread_cond_broadcast(&pshareRes->cond);
    printf("consurme1 consume %d,now goods num is %d\n",dtn,pshareRes->size);
    pthread_mutex_unlock(&pshareRes->mutex);
    sleep(1);
    }
   
    return NULL;
}
void * consumerthread2(void *arg){
    shareRes_t *pshareRes=(shareRes_t *)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->size<=MINSIZE){
        printf("consumer2 find goods now is not enough!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        printf("consumer2 is wake up now!\n");
    }
    //--pshareRes->size;
    int dtn=headDelete(pshareRes);
    pthread_cond_broadcast(&pshareRes->cond);
    printf("consurme2 consume %d,now goods num is %d\n",dtn,pshareRes->size);
    pthread_mutex_unlock(&pshareRes->mutex);
    sleep(1);
    }
    
    return NULL;
}


int main(){

    shareRes_t shareRes;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);

    pthread_t ptid1,ptid2,ptid3;
    pthread_t ctid1,ctid2;

    
    //初始化链表，定义随机值
    printf("Listnode begain init.\n");
    initList(&shareRes);
    srand(time(NULL));

    printf("Listnode begain insert.\n");
    for(int i=0;i<8;i++){
        int data=rand()%10+1;
        insertList(&shareRes);
    }
    printf("At begin goods have this.\n");
    printList(&shareRes);

   
  

    printf("produce thread begin create!\n");
    pthread_create(&ptid1,NULL,producethread1,&shareRes);
    pthread_create(&ptid2,NULL,producethread2,&shareRes);
    pthread_create(&ptid3,NULL,producethread3,&shareRes);

    sleep(5);
    printf("consumer thread begin create!\n");
    pthread_create(&ctid1,NULL,consumerthread1,&shareRes);
    pthread_create(&ctid2,NULL,consumerthread2,&shareRes);

    pthread_join(ptid1,NULL);
    pthread_join(ptid2,NULL);
    pthread_join(ptid3,NULL);
    pthread_join(ctid1,NULL);
    pthread_join(ctid2,NULL);

    return 0;

}
