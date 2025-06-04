#include <my_header.h>
#define TARGET 1000000
/* Usage:  */

long count=0;
int fd;
void handler(int signum){
    char ch='a';
    if(write(fd,&ch,1)==1){
        count++;
    }
}
int main(int argc, char *argv[]){                                  
     fd=open("test.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(-1,fd,"OPEN ERROR\n");

    signal(SIGALRM,handler);

    struct itimerval itv;
    itv.it_value.tv_sec=0;
    itv.it_value.tv_usec=10000;
    itv.it_interval.tv_sec=0;
    itv.it_interval.tv_usec=1000;
    setitimer(ITIMER_REAL,&itv,NULL);

    while(count <=TARGET){}

    printf("write TARGET need time is %ld\n",count);
    close(fd);
    
    return 0;
}

