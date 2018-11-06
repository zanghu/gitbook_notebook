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

以下的服务端代码使用python2，客户端代码使用python3。

本节的代码曾被用来批量下载部署在服务器上的图片数据，基本思路是：

（1）客户端将图片二进制读取后使用base64编码得到b64；
（2）b64前面再加上图片名称（32位字符串）得到body；
（3）body前面加上16位表示后面内容长度的报文头，报文头每个字符是0-9的数字，左侧补零，值为body长度。

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
#coding=utf-8
#!/bin/env python
import os
import base64
import socket
import numpy
import time

m_serv_ip = '10.230.147.31'
m_serv_port = 9999

def init_socket(serv_ip, serv_port):
    """"""
    ip_port = (serv_ip, serv_port)
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) # TCP
    #sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0) # UDP
    sk.connect(ip_port)
    sk.settimeout(50)
    return sk

def send_socket(sk, b64, name):
    """"""
    len_content = len(b64) + len(name)
    sk.sendall(bytes(str(len_content).zfill(16), encoding='utf-8')) # 发送头部
    sk.sendall(bytes(name, encoding='utf-8'))
    #sk.sendall(str(len_content).zfill(16)) # 发送头部
    #sk.sendall(name)
    sk.sendall(b64) # 发送内容
    sk.close()

def img_to_b64(img_path):
    """显示一副图片"""
    assert os.path.isfile(img_path)
    with open(img_path, 'rb') as f:
        img = f.read()
    b64 = base64.b64encode(img)
    return b64

def get_img_names(img_dir):
    """"""
    assert os.path.isdir(img_dir)
    names_all = os.listdir(img_dir)
    names = [name for name in names_all if name.endswith('.jpg')]
    print('目录 {0} 下文件总数: {1}, 图片总数: {2}'.format(img_dir, len(names_all), len(names))) 
    return names
    
def send_batch(img_dir, img_names, start_idx, batch_num=10):
    """显示指定目录下的所有图片"""
    global m_serv_ip
    global m_serv_port
    t0 = time.clock()
    t1 = time.clock()
    for cnt, img_name in enumerate(img_names[start_idx: start_idx + batch_num]):
        img_path = os.path.join(img_dir, img_name)
        b64 = img_to_b64(img_path) # 获得b64编码
        sk = init_socket(m_serv_ip, m_serv_port)
        send_socket(sk, b64, img_name.rstrip('.jpg')) # 发送数据
        t2 = time.clock()
        print('cnt {0} finish, time elapsed: {1}, total elapsed: {2}'.format(cnt, t2 - t1, t2 - t0))
        t1 = t2
    
    print('all finished, num send: {0}, time elapsed: {1}'.format(len(img_names), time.clock() - t0))
    #sk.close()
    
def client(img_dir, batch_size, max_batch):
    """"""
    assert os.path.isdir(img_dir)
    t0 = time.clock()
    img_names = get_img_names(img_dir)
    num_img = len(img_names)
    num_finish = 0 # 已完成个数
    start_idx = 0 # batch起始索引号
    num_batch = 0
    while num_finish < num_img:
        max_num = 0
        num_left = num_img - num_finish
        if num_left < batch_size:
            max_num = num_left
        else:
            max_num = batch_size
        send_batch(img_dir, img_names, start_idx, max_num)
        start_idx += max_num
        num_finish += max_num
        num_batch += 1
        if num_batch >= max_batch:
            break
    print('client finish, time elapsed: {0}'.format(time.clock() - t0))

if __name__ == '__main__':
    client('../data/problem3/train', batch_size=20, max_batch=10000)
```



