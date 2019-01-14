#coding=utf-8
#!/bin/env python

import socket
  
  
if __name__ == '__main__':

    ip_port = ('127.0.0.1',9999)

    sk = socket.socket(socket.AF_INET,socket.SOCK_DGRAM,0)
    sk.bind(ip_port)

    while True:
        data = sk.recv(1024)
        print(data.decode())