#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

#include "test_debug_macros.h"

// 服务器监听地址
static const int s_serv_port = 8008;

int invokeServer(int *sock, int serv_port)
{
    CHK_NIL(sock);

    struct sockaddr_in my_addr;   //服务器网络地址结构体

    memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
    my_addr.sin_family=AF_INET; //设置为IP通信
    my_addr.sin_addr.s_addr=INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
    my_addr.sin_port=htons(serv_port); //服务器端口号

    // 1.创建服务器端套接字--IPv4协议，面向连接通信，TCP协议
    if((*sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        int err = errno;
        fprintf(stderr, "socket() failed, %s, error.\n", strerror(err));
        return TEST_ERROR;
    }
    // 允许端口重用
    int reuse = 0;
    if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
        int err = errno;
        fprintf(stderr, "setsockopt() failed, %s, error.\n", strerror(err));
        return TEST_ERROR;
    }
 
    // 2.将套接字绑定到服务器的网络地址上
    if (bind(*sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) != 0) {
        int err = errno;
        fprintf(stderr, "bind() failed, %s, error.\n", strerror(err));
        return TEST_ERROR;
    }
    
    // 3.监听连接请求--第二个参数是监听队列长度
    // 据说内核参数默认值是128, 对于负载很大的服务, 建议改为2048或更高
    if (listen(*sock, 2048) != 0) {
        int err = errno;
        fprintf(stderr, "listen() failed, %s, error.\n", strerror(err));
        return TEST_ERROR;
    }

    return 0;
}

int test_serv()
{
    // 1.启动服务端
    int serv_sock = -1;
    CHK_ERR(invokeServer(&serv_sock, s_serv_port));
    fprintf(stdout, "服务端已启动, 监听端口: %d\n", s_serv_port);

    // 2.等待服务端连接
    struct sockaddr_in remote_addr; //客户端网络地址结构体
    socklen_t sin_size=sizeof(struct sockaddr_in);

    int client_sock = -1;
    if((client_sock = accept(serv_sock, (struct sockaddr *)&remote_addr, &sin_size)) < 0) {
        int err = errno;
        fprintf(stderr, "accept() failed, %s, error.\n", strerror(err));
        return TEST_ERROR;
    }
    fprintf(stdout, "客户端连接成功，accept client %s\n",inet_ntoa(remote_addr.sin_addr));

    // 3.等待客户端send超时
    sleep(300);

    CHK_ERR(close(client_sock));
    CHK_ERR(close(serv_sock));

    return 0;
}

int main()
{
    CHK_ERR(test_serv());
    return 0;
}
