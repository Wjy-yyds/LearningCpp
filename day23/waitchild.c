#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    pid_t pid =fork();
    if(pid==0){
        printf("children sleep 10 s\n");
        sleep(5);
        printf("children over\n");
        exit(0);
    }else{
        printf("father is waiting children now.\n");
        time_t start_time=time(NULL);
        int status;
        pid_t result;
        while((time(NULL)-start_time)<20){
            result=waitpid(pid,&status,WNOHANG);
            if(result==0){
                printf("children is still running\n");
                sleep(1);
                continue;
            }else if(pid >0){
                if(WIFEXITED(status)){
                    printf("children out now\n");
                }
            }
    }
    return 0;
    }
}

