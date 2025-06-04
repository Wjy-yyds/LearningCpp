#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd[2];
    pipe(fd);
    
    // 获取fd[0]的文件状态标志
    int flags = fcntl(fd[0], F_GETFL);
    printf("管道读端标志: %d\n", flags);
    
    pid_t pid = fork();
    if (pid == 0) {
        // 子进程
        struct stat st;
        fstat(fd[0], &st);
        printf("子进程 - inode: %lu\n", st.st_ino);
        printf("子进程 - 文件描述符: %d, %d\n", fd[0], fd[1]);
    } else {
        // 父进程
        struct stat st;
        fstat(fd[0], &st);
        printf("父进程 - inode: %lu\n", st.st_ino);
        printf("父进程 - 文件描述符: %d, %d\n", fd[0], fd[1]);
    }
    
    return 0;
}
