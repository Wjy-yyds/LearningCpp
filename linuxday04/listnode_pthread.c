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

    //开始创建链表并打印
    printf("thread begin add node\n");
    ListNode *head=shareRes->head;
    ListNode *tail=shareRes->tail;
    for(int i=1;i<5;i++){
        ListNode *node=(ListNode *)malloc(sizeof(ListNode));
        node->data=i;
        node->next=NULL;
        //这里不能借助新的head和tail来操作
        //
        shareRes->tail->next=node;
        shareRes->tail=node;
    }
    while(head!=NULL){
        if(head->next!=NULL){
            printf("%d",head->data);
            printf("->");
            head=head->next;
        }else{
            printf("%d\n",head->data);
            head=head->next;
        }
    }
    printf("thread add node success\n");

    pthread_mutex_lock(&shareRes->mutex);
    shareRes->flag=2;
    pthread_cond
    pthread_cond_signal(&shareRes->cond);
    pthread_mutex_unlock(&shareRes->mutex);
    //链表操作完成后，通知主线程

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


    printf("main thread begin create listnode\n");
    ListNode *head=(ListNode *)malloc(sizeof(ListNode));
    head->data=0;
    head->next=NULL;
    shareRes.tail=head;
    shareRes.head=head;
    printf("main thread create listnode success\n");

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

    printf("main pthread begin detlet\n");
    //链表操作完成后，主线程才允许继续执行
    ListNode *currnode=shareRes.head;
    while(currnode!=NULL){
        ListNode *tmp=currnode;
        currnode=currnode->next;
        free(tmp);
    }
    shareRes.head = NULL;  // 防止后续误访问
    shareRes.tail = NULL;
    printf("main pthread delete listnode success\n");
    
    return 0;
}
