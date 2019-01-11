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
int test_send_timeout(const char *req, int len_req, const char *serv_ip, int serv_port, int timeout_sec)
{
    CHK_NIL(req);
    CHK_ERR((len_req > 0)? 0: 1);
    CHK_NIL(serv_ip);
    CHK_ERR((timeout_sec >= 0)? 0: 1);

    // 1.建立连接
    int sock = 0;
    CHK_ERR(connectToServer(&sock, serv_ip, serv_port));

    // 2.设置超时时间
    struct timeval timeout;
    unsigned int len_x = sizeof(timeout);
    CHK_ERR(getsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, &len_x));
    fprintf(stdout, "default SO_SNDTIMEO: %lu.%lus\n", timeout.tv_sec, timeout.tv_usec * 1000000);
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    CHK_ERR(setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval)));
    fprintf(stdout, "send超时设置成功.\n");

    // 3.发送请求接收返回
    int cnt = 0;
    do {
    int error = sendMsg(sock, req, len_req);
    if (error == -1) {
        fprintf(stderr, "sendMsg() failed, error.\n");
    }
    fprintf(stdout, "send() success, cyc %d\n", cnt);
    ++cnt;

    } while (1);


    // 关闭连接
    CHK_ERR(close(sock));

    return 0;
}


int main()
{
    CHK_ERR(test_send_timeout("hello robot!abcd", 16, "127.0.0.1", 8008, 5));
    //CHK_ERR(test_recv_timeout("127.0.0.1", 8008, 5));

    return 0;
}


