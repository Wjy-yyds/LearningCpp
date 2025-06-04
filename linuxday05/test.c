#include "my_header.h"
typedef struct shareRes_s {
    int food; //当前的食物
    pthread_mutex_t mutex; // 锁
    pthread_cond_t cond; // 条件变量 （队列）
} shareRes_t;
// 1 黄焖鸡 2 猪脚饭
void *student1(void *arg){
    shareRes_t *pshareRes = (shareRes_t *)arg;
    pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->food != 1){ // if --> while 兼容虚假唤醒
        printf("学生发现没有黄焖鸡!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        printf("学生1 被喊醒了!\n");
    }
    // 黄焖鸡到了
    pshareRes->food = 0; // 吃掉了
    pthread_mutex_unlock(&pshareRes->mutex);
    return NULL;
}
void *student2(void *arg){
    shareRes_t *pshareRes = (shareRes_t *)arg;
    pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->food != 2){
        printf("学生发现没有猪脚饭!\n");
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        printf("学生2 被喊醒了!\n");
    }
    // 猪脚饭到了
    pshareRes->food = 0; // 吃掉了
    pthread_mutex_unlock(&pshareRes->mutex);
    return NULL;
}
int main(int argc, char *argv[])
{
    shareRes_t shareRes;
    shareRes.food = 0; //没食物
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);
    
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,student1,&shareRes);
    pthread_create(&tid2,NULL,student2,&shareRes);

    while(1){
        int food;
        scanf("%d", &food);

        pthread_mutex_lock(&shareRes.mutex);
        shareRes.food = food;
        if(food == 0){
            printf("逗你玩!\n");
        }
        else if(food == 1){
            printf("食堂来了黄焖鸡!\n");
        }
        else if(food == 2){
            printf("食堂来了猪脚饭!\n");
        }
        pthread_cond_broadcast(&shareRes.cond);
        pthread_mutex_unlock(&shareRes.mutex);
    }
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}

