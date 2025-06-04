#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){

    ARGS_CHECK(argc,2);
    struct hostent *host=gethostbyname(argv[1]);
    if(host == NULL){
        herror("gethostbyname");
        exit(-1);
    }
    
    printf("官方名字是：%s\n",host->h_name);
    for(int i=0;host->h_aliases[i]!=NULL;i++){
        printf("别名为:%s\n",host->h_aliases[i]);
    }

    

    return 0;
}

