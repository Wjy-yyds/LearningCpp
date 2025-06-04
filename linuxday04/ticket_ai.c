#include "my_header.h"

typedef struct shareRes_s {
    int ticket_count;
    pthread_mutex_t mutex;
} shareRes_t;

void* sale1(void* arg) {
    shareRes_t* shareRes = (shareRes_t*)arg;
    
    while (1) {
        pthread_mutex_lock(&shareRes->mutex);
        
        // 检查票数并售票
        if (shareRes->ticket_count <= 0) {
            pthread_mutex_unlock(&shareRes->mutex);
            break;
        }
        
        printf("[窗口1] 售出第%d张票，剩余：%d张\n", 
              21 - shareRes->ticket_count, 
              shareRes->ticket_count - 1);
        
        shareRes->ticket_count--;
        pthread_mutex_unlock(&shareRes->mutex);  // 先解锁
        
        sleep(1);  // 延迟放在锁外
    }
    
    pthread_exit(NULL);
}

void* sale2(void* arg) {
    shareRes_t* shareRes = (shareRes_t*)arg;

    while (1) {
        pthread_mutex_lock(&shareRes->mutex);
        
        if (shareRes->ticket_count <= 0) {
            pthread_mutex_unlock(&shareRes->mutex);
            break;
        }
        
        printf("[窗口2] 售出第%d张票，剩余：%d张\n", 
              21 - shareRes->ticket_count, 
              shareRes->ticket_count - 1);
        
        shareRes->ticket_count--;
        pthread_mutex_unlock(&shareRes->mutex);  // 先解锁
        
        sleep(1);  // 延迟放在锁外
    }

    pthread_exit(NULL);
}

int main() {
    shareRes_t shareRes;
    shareRes.ticket_count = 20;
    pthread_mutex_init(&shareRes.mutex, NULL);

    pthread_t ptid1, ptid2;
    struct timeval begTime, endTime;
    
    // 记录开始时间
    gettimeofday(&begTime, NULL);
    
    // 创建线程
    pthread_create(&ptid1, NULL, sale1, &shareRes);
    pthread_create(&ptid2, NULL, sale2, &shareRes);

    // 等待线程结束
    pthread_join(ptid1, NULL);
    pthread_join(ptid2, NULL);
    
    // 计算总耗时
    gettimeofday(&endTime, NULL);
    printf("票已售罄！总耗时：%ld秒\n", endTime.tv_sec - begTime.tv_sec);
    
    // 清理资源
    pthread_mutex_destroy(&shareRes.mutex);
    
    return 0;
}
