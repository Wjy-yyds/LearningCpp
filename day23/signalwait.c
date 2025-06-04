#include <my_header.h>

/* Usage:  */

int main(int argc, char *argv[]){
    char str[27]={0};
    sigset_t curset,oldset;
    
    sigemptyset(&curset);
    sigaddset(&curset,SIGINT);
    sigprocmask(SIG_BLOCK,&curset,&oldset);

    for(int i=0;i<26;i++){
        sleep(1);
        str[i]='a'+i;
    }
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending,SIGINT)){
        printf("SIGINT receive\n");

    }else{
        printf("NOT RECEIVE\n");
    }
    sigprocmask(SIG_SETMASK,&oldset,NULL);
    return 0;
}

