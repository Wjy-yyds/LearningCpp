#include "my_header.h"

int main(){

    printf("开始重定向代码：\n");

    int stdout_back;
    //保存标准输出的文件描述符，并让stdout_back指向它
    dup2(STDOUT_FILENO,stdout_back);

    //关闭标准输出
    int fd=open("test03.txt",O_RDWR|O_CREAT|O_TRUNC,0664);
    close(STDOUT_FILENO);
    //将fd复制到标准输出
    dup(fd);
    printf(" stdout to test03 \n");


    close(fd);
    //恢复标准输出
    dup2(stdout_back,STDOUT_FILENO);
    printf("stdout reback\n");
    close(stdout_back);

    return 0;


}
