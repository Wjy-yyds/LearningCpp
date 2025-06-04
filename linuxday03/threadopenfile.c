#include <my_header.h>

/* Usage:  */


void closefd1(void *arg){
    int *pfd1=(int *)arg;
    printf("close fd1\n");
    close(*pfd1);
}
void closefd2(void *arg){
    int *pfd2=(int *)arg;
    printf("close fd2\n");
    close(*pfd2);
}
void closefd3(void *arg){
    int *pfd3=(int *)arg;
    printf("close fd3\n");
    close(*pfd3);
}
void * threadFunc(void *arg){
    int fd1=open("file1.txt",O_RDWR|O_CREAT,0666);
    pthread_cleanup_push(closefd1,&fd1);
    int fd2=open("file2.txt",O_WRONLY|O_APPEND|O_CREAT,0666);
    pthread_cleanup_push(closefd2,&fd2);
    int fd3=open("file3.txt",O_WRONLY|O_APPEND|O_CREAT,0666);
    ERROR_CHECK(fd3,-1,"open error\n");
    pthread_cleanup_push(closefd3,&fd3);
    
    char buf1[1000]={0};
    size_t n=0;
    while((n=read(fd1,buf1,sizeof(buf1)))>0){
        write(fd2,buf1,n);
    }
    lseek(fd2,0,SEEK_SET);

    while((n=read(fd2,buf1,sizeof(buf1)))>0){write(fd3,buf1,n);  }
    




    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    return NULL ;

}
int main(int argc, char *argv[]){                                  
    pthread_t tid;
    pthread_create(&tid,NULL,threadFunc,NULL);

    pthread_join(tid,NULL);



    return 0;
}

