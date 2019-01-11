#pragma once

// 与服务端建立连接
int connectToServer(int *client_socket, const char *serv_ip, int serv_port);

// 发送请求
int sendMsg(int sock, const char *msg_in, int len_in);

// 接收响应
int recvMsg(char **msg_in, int *len_in, int sock);


