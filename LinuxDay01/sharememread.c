#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int sharememid=shmget(0x1234,4096,IPC_CREAT|0666);
    ERROR_CHECK(sharememid,-1,"READ SHARE ERROR\n");

    char *p=(char *)shmat(sharememid,NULL,0);
    ERROR_CHECK(p,NULL,"ERROR SHMAT\n");

    printf("write process send is %s.\n",p);
    shmdt(p);


    return 0;
}

