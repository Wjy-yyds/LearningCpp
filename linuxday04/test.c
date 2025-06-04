#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_TICKETS 20

// 全局变量
int remaining_tickets = TOTAL_TICKETS;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 售票函数
void* sell_ticket(void* arg) {
    int window_id = *(int*)arg;
    
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);
        
        // 检查剩余票数
        if (remaining_tickets > 0) {
            printf("窗口 %d 正在售出第 %d 张票\n", window_id, TOTAL_TICKETS - remaining_tickets + 1);
            remaining_tickets--;
            pthread_mutex_unlock(&mutex);
            sleep(1);  // 模拟售票过程
        } else {
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    
    printf("窗口 %d 票已售完！\n", window_id);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int window1 = 1, window2 = 2;
    
    // 创建两个售票线程
    pthread_create(&thread1, NULL, sell_ticket, &window1);
    pthread_create(&thread2, NULL, sell_ticket, &window2);
    
    // 等待两个线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);
    
    printf("所有票已售完！\n");
    return 0;
}