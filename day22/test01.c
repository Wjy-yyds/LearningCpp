#include "my_header.h"

int main(){
    //用1只读
    //用2只写
    int pipe_read=open("1.pipe",O_RDONLY);
    int pipe_write=open("2.pipe",O_WRONLY);
    ERROR_CHECK(pipe_read,-1," test01 open pipe_read failed");
    ERROR_CHECK(pipe_write,-1,"test02 open pipe_write failed");


    char buf[128]={0};
    //printf("test01 after read:\n");
    read(pipe_read,buf,sizeof(buf));
    printf("test01 after read buf=%s\n",buf);

   // printf("test01 after write:\n");
    char *str="test01 write buf";
    write(pipe_write,str,strlen(str));
    printf("test01 after write buf=%s\n",str);

    close(pipe_read);
    close(pipe_write);
    return 0;

}