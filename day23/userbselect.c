#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int read_fd = open("1.pipe",O_RDONLY);
    int write_fd = open("2.pipe",O_WRONLY);
    
    fd_set readfds;
    char buf[1024]={0};

    while(1){

        FD_ZERO(&readfds);

        FD_SET(STDIN_FILENO,&readfds);
        FD_SET(read_fd,&readfds);

        int ret = select(10,&readfds,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&readfds)){
            memset(buf,0,sizeof(buf));
            int len = read(STDIN_FILENO
                           ,buf,sizeof(buf));
             write(write_fd,buf,len);    
        }
        if(FD_ISSET(read_fd,&readfds)){
            memset(buf,0,sizeof(buf));
            int len = read(read_fd
                               ,buf,sizeof(buf));
            printf("UserA say:%s\n",buf);
        }
    }
    close(write_fd);
    close(read_fd);

    return 0;
}

