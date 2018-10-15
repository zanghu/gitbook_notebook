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


// 与服务端建立连接
int connectToServer(int *client_socket, const char *serv_ip, int serv_port)
{
    CHK_NIL(client_socket);
    CHK_NIL(serv_ip);

    int error = 0;

    // 1.socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        int err = errno;
        fprintf(stderr, "socket() failed, %s, error.\n", strerror(err));
        return TEST_ERROR;
    }

    do {

    // 2.connect
    struct sockaddr_in sock_addr;
    bzero(&sock_addr, (int)sizeof(sock_addr));     
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = (serv_ip? inet_addr(serv_ip): htonl(INADDR_ANY));
    sock_addr.sin_port = htons(serv_port);

    error = connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
    if (error != 0) {
        int err = errno;
        fprintf(stderr, "connect() failed, %s, error.\n", strerror(err));
        break;
    }

    } while (0);

    if (error != 0) {
        close(sock);
        return TEST_ERROR;
    }
    *client_socket = sock;

    return 0;
}


int sendMsg(int sock, const char *msg_in, int len_in)
{
    CHK_NIL(msg_in);
    CHK_ERR((len_in > 0)? 0: 1);

    int offset = 0;
    int len_write = 0;
    int cnt = 0;
    while (offset < len_in) {
        len_write = send(sock, msg_in + offset, len_in - offset, 0);
        if (len_write == -1) {
            int err = errno;
            fprintf(stderr, "send() failed, errno: %d, %s, error.\n", err, strerror(err));
            break;
        }
        offset += len_write;
        ++cnt;
    }
    fprintf(stdout, "sendMsg() cyc num: %d\n", cnt);

    return 0;
}

int recvMsg(char **msg_in, int *len_in, int sock)
{
    CHK_NIL(msg_in);
    CHK_NIL(len_in);      

    char *szReadBuf = NULL;
    CHK_NIL((szReadBuf = calloc(1024, sizeof(char))));
    int iReadLen;

    int cnt = 0;
    int offset = 0;
    while(1) {
        iReadLen = recv(sock, szReadBuf + offset, 1024, 0);
        if (iReadLen <= 0) {
            int err = errno;
            if (iReadLen == 0) {
                fprintf(stderr, "服务端主动断开连接, 循环次数 %d, errno: %d, %s, error.\n", cnt, err, strerror(err));
                break; // 正常退出
            }
            else {
                fprintf(stderr, "recv() failed, errno: %d, %s, error.\n", err, strerror(err));
                return TEST_ERROR;
            }
        }
        offset += iReadLen;
        ++cnt;
    }
    fprintf(stdout, "返回报文接收完成.\n");

    *msg_in = szReadBuf;
    *len_in = offset;

    return 0;
}