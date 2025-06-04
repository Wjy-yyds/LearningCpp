#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int father_to_son[2];
    pipe(father_to_son);
    int son_to_father[2];
    pipe(son_to_father);

    if(fork()==0){
        char buf[64]={0};
        close(father_to_son[1]);
        read(father_to_son[0],buf,sizeof(buf));
        printf("Childen receive father messgae is %s\n",buf);

        char *buf2="this is children message";
        close(son_to_father[0]);
        write(son_to_father[1],buf2,strlen(buf2));
        close(father_to_son[0]);
        close(son_to_father[1]);
        exit(0);

    }else{

        close(father_to_son[0]);
        char *buf="This is father send to son\n";
        write(father_to_son[1],buf,strlen(buf));
        
        
        char buff[64]={0};
        close(son_to_father[1]);
        read(son_to_father[0],buff,sizeof(buff));
        close(father_to_son[1]);
        close(son_to_father[0]);
    }
    return 0;
}

