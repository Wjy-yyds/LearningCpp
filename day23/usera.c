#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int write_fd = open("1.pipe",O_WRONLY);
    int read_fd = open("2.pipe",O_RDONLY);

    while(1){
        char buf[1024]={0};

        printf("usera ,please input your message:\n");
        int read_count=read(STDIN_FILENO,
                            buf,sizeof(buf));
        if(read_count<=0){
            printf("read_count failed\n");
        }

        write(write_fd,buf,read_count);

        memset(buf,0,sizeof(buf));
        read_count=read(read_fd,buf,sizeof(buf));
        printf("usera read from userb message:%s\n",buf);
    
    }
    close(write_fd);
    close(read_fd);


    return 0;
}

