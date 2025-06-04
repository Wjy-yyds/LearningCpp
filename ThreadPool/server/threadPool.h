#ifndef __THREAD_POOL__
#define __THREAD_POOL__
#include <my_header.h>
#include "tidArr.h"
#include "taskQueue.h"

typedef struct threadPool_s{

    tidArr_t tidArr;

    taskQueue_t taskQueue;

    pthread_mutex_t mutex;

    pthread_cond_t cond;

    int exitFlag;
}threadPool_t;

//初始化线程池时，使用传入传出参数并且确定线程数量
int threadPoolInit(threadPool_t *pthreadPool,int workNum);

//线程创造时需要的参数，线程可以访问主线程内容，直接把整个结构体给他
int makeWorker(threadPool_t *pthreadPool);

int tcpInit();

int epollAdd(int epfd,int fd);

int epollDel(int epfd,int fd);

int transfile(int sockfd);
#endif
