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

```

**客户端**

```python

```



