#include <my_header.h>
#define NUM  10000000000
/* Usage:  */
int main(int argc, char *argv[]){

    int shmid = shmget(0x1111,4096,IPC_CREAT|0666);
    ERROR_CHECK(shmid,-1,"shmget error\n");
    
    long *p=(long *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,NULL,"shmat ERRor\n");

    p[0]=0;
    if(fork()==0){
        for(long i=0;i<NUM;i++){
            ++p[0];
        }
        exit(0);
    }else{
        for(long i=0;i<NUM;i++){
            ++p[0];
        }
        wait(NULL);
        printf("The final sum is %ld \n",p[0]);
    }

    shmdt(p);
    return 0;
}

