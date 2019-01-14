#coding=utf-8
#!/bin/env python

import socket
import time
import os

m_serv_ip = '10.230.147.31'
m_serv_port = 9999

m_len_header = 16
m_len_buf = 4096

m_save_dir = 'b64'


def read_header(header):
    """"""
    global m_len_header
    content = header[:m_len_header].lstrip('0')
    assert content != ''
    return eval(content)
  
  
def server():
    """"""
    global m_serv_ip
    global m_serv_port
    global m_len_header
    global m_len_buf
    global m_save_dir

    t0 = time.clock()
    ip_port = (m_serv_ip, m_serv_port)

    #sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM,0) # UDP
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM,0) # TCP
    sk.bind(ip_port)
    sk.listen(5) # 一般都填5, 内核排队数量

    cnt = 0
    while True:
        conn, addr = sk.accept()
        data = conn.recv(m_len_header)
        len_data = read_header(data)
        print 'header: {0}'.format(len_data)
        contents = []
        num_cyc = 0
        len_left = len_data
        while len_left > 0:
            print 'start: {0}'.format(num_cyc)
            #data = sk.recv(len_buf)
            data = conn.recv(m_len_buf)
            contents.append(data)
            len_left -= m_len_buf
            print 'num_cyc = {0}, data:\n{1}'.format(num_cyc, data)
            num_cyc += 1
            #break
        conn.close()

        if not os.path.isdir(m_save_dir):
            os.mkdir(m_save_dir)
        save_path = os.path.join(m_save_dir, str(cnt).zfill(5))
        with open(save_path, 'wb') as f:
            f.write(''.join(contents))
        cnt += 1
        print 'num finish: {0} finish, num bytes: {1}, time elapsed: {2}'.format(cnt, len_left, time.clock() - t0)


if __name__ == '__main__':
    server()