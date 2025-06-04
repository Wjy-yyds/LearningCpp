#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){ 
    int read_fd=open("1.pipe",O_RDONLY);
    int write_fd=open("2.pipe",O_WRONLY);

    while(1){
        // b receive a
        char buf[1024]={0};
        int read_count=read(read_fd,buf,sizeof(buf));
        printf("b read message from a is %s \n",buf);
        
        // b send a
        memset(buf,0,sizeof(buf));
        printf("please userb input message:\n");
        read_count=read(STDIN_FILENO,buf,sizeof(buf));
        write(write_fd,buf,read_count);



    }
    close(write_fd);
    close(read_fd);
    return 0;
}

