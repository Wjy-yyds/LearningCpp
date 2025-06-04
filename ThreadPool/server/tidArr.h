#ifndef __TID_ARR__
#define __TID_ARR__
#include <my_header.h>
//这是线程数组
typedef struct tidArr_s{
    int workerNum;
    pthread_t *arr;//注意指针在初始化的时候必须申请内存
                   //否则用数组
}tidArr_t;

int tidArrInit(tidArr_t *pidArr,int workerNum);
#endif
