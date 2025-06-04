#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    pid_t pid=fork();
    if(pid==0){
        
        printf("child running ,pid is %d\n",getpid());
        exit(0);

    }else{
        printf("Parent process running,pid is %d\n",getpid());
        sleep(5);
    }
    return 0;
}

