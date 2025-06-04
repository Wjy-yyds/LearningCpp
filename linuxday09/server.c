#include <my_header.h>
int main(int argc, char *argv[])
{
    // ./03_server 192.168.72.128 12345
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    // 赋值使用具体类型
    struct sockaddr_in addr;
    addr.sin_family = AF_INET; //IPv4
    addr.sin_port = htons(atoi(argv[2])); // 字符串-->小端整数-->大端整数
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    // 传参用公共类型
    int ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)); // 先取地址，再强转
    ERROR_CHECK(ret,-1,"bind");
    listen(sockfd,50);
    // 想要获取客户端的ip和端口
    struct sockaddr_in clientAddr;
    //卡在accept
    socklen_t socklen = sizeof(clientAddr); //socklen初始值不能是0
    int netfd = accept(sockfd,(struct sockaddr *)&clientAddr,&socklen);
    // 从全连接队列里面取出一条连接（FIFO），并获取客户端的ip和端口
    printf("netfd = %d, port = %d, ip = %s\n", netfd, 
           ntohs(clientAddr.sin_port), inet_ntoa(clientAddr.sin_addr));

    //sleep(1);
    //printf("sleep over!\n");
    //// 服务端 sockfd是监听socket netfd是通信socket
    //char buf[4096] = {0};
    //ssize_t sret = recv(netfd,buf,sizeof(buf),0);
    //printf("buf = %s, sret = %ld\n", buf, sret);
    
    //close(netfd);
    //printf("netfd has closed!\n");
    //while(1){
    //    sleep(1);
    //}
    
    char buf[4096] = {0};
    read(netfd,buf,sizeof(buf));
    printf("buf = %s\n", buf);
    return 0;
}

