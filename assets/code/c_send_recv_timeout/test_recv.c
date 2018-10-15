#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "test_debug_macros.h"
#include "client_socket.h"


// 发送请求接收响应
int test_recv_timeout(const char *serv_ip, int serv_port, int timeout_sec)
{
    CHK_NIL(serv_ip);
    CHK_ERR((timeout_sec >= 0)? 0: 1);

    // 1.建立连接
    int sock = 0;
    CHK_ERR(connectToServer(&sock, serv_ip, serv_port));

    // 2.设置超时时间
    struct timeval timeout;
    unsigned int len_x = sizeof(timeout);
    CHK_ERR(getsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, &len_x));
    fprintf(stdout, "default SO_RCVTIMEO: %lu.%lus\n", timeout.tv_sec, timeout.tv_usec * 1000000);
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    CHK_ERR(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)));
    fprintf(stdout, "recv超时设置成功.\n");
    len_x = sizeof(timeout);
    CHK_ERR(getsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, &len_x));
    fprintf(stdout, "default SO_RCVTIMEO: %lu.%lus\n", timeout.tv_sec, timeout.tv_usec * 1000000);

    // 3.发送请求接收返回
    
    int cnt = 0;
    char *res = NULL;
    int len_res = 0;
    do {
    int error = recvMsg(&res, &len_res, sock);
    if (error == -1) {
        fprintf(stderr, "recvMsg() failed, error.\n");
    }
    fprintf(stdout, "recv() success, cyc %d\n", cnt);
    ++cnt;

    } while (1);


    // 关闭连接
    CHK_ERR(close(sock));

    return 0;
}


int main()
{
    CHK_ERR(test_recv_timeout("127.0.0.1", 8008, 5));

    return 0;
}


