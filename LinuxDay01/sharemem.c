#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int share_mem_id=shmget(0x1234,4096,IPC_CREAT|0666);
    ERROR_CHECK(share_mem_id,-1,"Share Memory ERROR\n");

    char *p=(char *)shmat(share_mem_id,NULL,0);
    ERROR_CHECK(p,NULL,"Shamt ERRO\n");

    strcpy(p,"How are you\n");
    shmdt(p);
    return 0;
}

