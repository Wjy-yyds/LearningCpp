#include "my_header.h"

int main(){

    printf("��ʼ�ض�����룺\n");

    int stdout_back;
    //�����׼������ļ�������������stdout_backָ����
    dup2(STDOUT_FILENO,stdout_back);

    //�رձ�׼���
    int fd=open("test03.txt",O_RDWR|O_CREAT|O_TRUNC,0664);
    close(STDOUT_FILENO);
    //��fd���Ƶ���׼���
    dup(fd);
    printf(" stdout to test03 \n");


    close(fd);
    //�ָ���׼���
    dup2(stdout_back,STDOUT_FILENO);
    printf("stdout reback\n");
    close(stdout_back);

    return 0;


}
