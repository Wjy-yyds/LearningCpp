#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    pid_t pid = fork();

    if(pid==0){
        printf("Child process begin now\n");
        printf("Child pid is %d\n",getpid());
        sleep(10);
        printf("Child run without father pid=%d\n",getppid());
    }else{
        printf("Partent proecss will exit.PID=%d\n",getpid());
        exit(0);
    }
    return 0;
}

