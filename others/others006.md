## 微服务常见发布模式介绍

参考资料1：[http://tshare365.com/archives/2805.html](http://tshare365.com/archives/2805.html)  
参考资料2：[https://blog.csdn.net/zyqduron/article/details/59507525](https://blog.csdn.net/zyqduron/article/details/59507525)

`curl`是一个非常有用的命令，可以用来模拟http请求，用来检查网络是否联通、服务是否正常等等。

很多情况下，`curl`远比`ping`或`telnet`更为简单有效。

### 1.观察整个通讯过程

使用`-v`参数可以显示一次 http 通信的整个过程，包括端口连接和 http request 头信息。甚至可以在使用代理的情况下打印出看到调用代理的过程。

下面的例子是service mesh软件envoy官方给出的通过docker运行envoy例子，用到了curl -v

```
$ docker pull envoyproxy/envoy:latest
$ docker run --rm -d -p 10000:10000 envoyproxy/envoy:latest
$ curl -v localhost:10000
```

### 2.下载资源同时重命名

使用`-o`参数在获取资源的同时保存到指定文件，相当于“指定保存路径 + 重命名”

下面的例子是minikube安装的官方例子的一部分

```
# 将下载的minikube-linux-amd64文件重命名为minikube保存到本地
curl -Lo minikube https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
```



