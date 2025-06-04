#include <my_header.h>
int main(int argc, char *argv[])
{
    // ./03_client 192.168.72.128 12345
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    // 赋值使用具体类型
    struct sockaddr_in addr; // 服务端地址
    addr.sin_family = AF_INET; //IPv4
    addr.sin_port = htons(atoi(argv[2])); // 字符串-->小端整数-->大端整数
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    // 传参用公共类型
    int ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)); // 先取地址，再强转
    ERROR_CHECK(ret,-1,"connect");

    //客户端只有sockfd是通信socket
    //sleep(5);
    //printf("sleep over!\n");
    //while(1){
    //    ssize_t sret = send(sockfd,"hello",5,0);
    //    printf("sret = %ld\n", sret);
    //}
    
    //close(sockfd);
    //printf("sockfd has closed!\n");
    //while(1){
    //    sleep(1);
    //}
    
    write(sockfd,"hello",5);
    return 0;
}

