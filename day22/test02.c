#include "my_header.h"

int main(){
    //”√1–¥
    //”√2∂¡
    int pipe_write=open("1.pipe",O_WRONLY);
    int pipe_read=open("2.pipe",O_RDONLY);
    ERROR_CHECK(pipe_read,-1,"test02 open pipe_read failed");
    ERROR_CHECK(pipe_write,-1,"test 02 open pipe_write failed");


    char * buf="test02 write buf";
    //printf("test02 after write:\n");
    write(pipe_write,buf,strlen(buf));
    printf("test02 after write buf=%s\n",buf);
    char read_buf[128]={0};
    //printf("test02 after read:\n");
    read(pipe_read,read_buf,sizeof(read_buf));
    printf("test02 after read buf=%s\n",read_buf);
    close(pipe_read);
    close(pipe_write);
    return 0;
    
}