# Python: Socket编程

Python提供了Socket编程接口，使用时只需`import socket`。

### 1.UDP编程示例

以下的代码都是基于python3，在python2下运行可能会有问题（客户端输入时加双引号似乎可以解决）。

**服务端**

```python
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
```

**客户端**

```python
#coding=utf-8
#!/bin/env python
import socket

if __name__ == '__main__':

    ip_port = ('127.0.0.1',9999)

    sk = socket.socket(socket.AF_INET,socket.SOCK_DGRAM,0)
    while True:
        inp = input('数据：').strip()
        if inp == 'exit':
            break
        sk.sendto(bytes(inp,encoding='utf8'),ip_port)

    sk.close()

```

### 2.TCP编程示例

以下的服务端代码使用python2，客户端代码使用python3

**服务端**

```python
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
```

**客户端**

```python

```



