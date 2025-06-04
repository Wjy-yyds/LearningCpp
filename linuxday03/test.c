#include "my_header.h"
void* threadFunc(void *arg) {
    int *result = (int *)malloc(sizeof(int));
    *result = 42;
    return (void*)result; // 返回动态分配的 int
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);

    void *retval; // 用于接收返回值
    pthread_join(tid, &retval); // 传递 retval 的地址

    int *result = (int*)retval;
    printf("Thread returned: %d\n", *result);
    free(result); // 释放线程分配的内存
    return 0;
}
