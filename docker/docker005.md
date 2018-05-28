## DockerFile的常用命令

参考资料：[https://www.cnblogs.com/dazhoushuoceshi/p/7066041.html](https://www.cnblogs.com/dazhoushuoceshi/p/7066041.html)

### VOLUME

* **功能**

实现挂载功能，可以将内地文件夹或者其他容器种得文件夹挂在到这个容器中

类似于命令 docker run -v

* **语法**

```
VOLUME ["/data"]
``` 

\["/data"\]可以是一个JsonArray ，也可以是多个值。所以如下几种写法都是正确的

```
VOLUME ["/var/log/"]
VOLUME /var/log
VOLUME /var/log /var/db
```

一般的使用场景为需要持久化存储数据时

容器使用的是AUFS，这种文件系统不能持久化数据，当容器关闭后，所有的更改都会丢失。

所以当数据需要持久化时用这个命令。

### USER

* **功能**

设置启动容器的用户，可以是用户名或UID

* **语法**

下面的两种写法是正确的

```
USER daemo
USER UID
```

* **注意**

注意：如果设置了容器以daemon用户去运行，那么RUN, CMD 和 ENTRYPOINT 都会以这个用户去运行

### WORKDIR

* **功能**
设置工作目录

* **语法**

```
WORKDIR /path/to/workdir
```

对RUN,CMD,ENTRYPOINT,COPY,ADD生效。如果不存在则会创建，也可以设置多次。如：

```
WORKDIR /a
WORKDIR b
WORKDIR c
RUN pwd
# pwd执行的结果是/a/b/c
```

WORKDIR也可以解析环境变量，如：

```
ENV DIRPATH /path
WORKDIR $DIRPATH/$DIRNAME
RUN pwd
# pwd的执行结果是/path/$DIRNAME
```
 
### ARG

* **功能**

设置变量命令

* **语法**

```
ARG <name>[=<default value>]
```

ARG命令定义了一个变量，在docker build创建镜像的时候，使用 --build-arg <varname>=<value>来指定参数

如果用户在build镜像时指定了一个参数没有定义在Dockerfile种，那么将有一个Warning

提示如下：

```
[Warning] One or more build-args [foo] were not consumed.
``` 

我们可以定义一个或多个参数，如下：

```
FROM busybox
ARG user1
ARG buildno
...
```

也可以给参数一个默认值：

```
FROM busybox
ARG user1=someuser
ARG buildno=1
...
```

如果我们给了ARG定义的参数默认值，那么当build镜像时没有指定参数值，将会使用这个默认值

### ONBUILD


* **功能**


* **语法**

```
ONBUILD [INSTRUCTION]
```

这个命令只对当前镜像的子镜像生效。

比如当前镜像为A，在Dockerfile种添加：

```
ONBUILD RUN ls -al
```

这个 ls -al 命令不会在A镜像构建或启动的时候执行

此时有一个镜像B是基于A镜像构建的，那么这个ls -al 命令会在B镜像构建的时候被执行。

 

 

### STOPSIGNAL


* **功能**

STOPSIGNAL命令是的作用是当容器推出时给系统发送什么样的指令


* **语法**

```
STOPSIGNAL signal
```

### HEALTHCHECK

* **功能**

 容器健康状况检查命令

* **语法**

```
# 语法1：在容器内部运行一个命令来检查容器的健康状况
HEALTHCHECK [OPTIONS] CMD command

# 语法2：在基础镜像中取消健康检查命令
HEALTHCHECK NONE
``` 

[OPTIONS]的选项支持以下三中选项：
```

    --interval=DURATION 两次检查默认的时间间隔为30秒

    --timeout=DURATION 健康检查命令运行超时时长，默认30秒

    --retries=N 当连续失败指定次数后，则容器被认为是不健康的，状态为unhealthy，默认次数是3

```

* **注意**

HEALTHCHECK命令只能出现一次，如果出现了多次，只有最后一个生效。

CMD后边的命令的返回值决定了本次健康检查是否成功，具体的返回值如下：

```

0: success - 表示容器是健康的

1: unhealthy - 表示容器已经不能工作了

2: reserved - 保留值
```
 

例子：

HEALTHCHECK --interval=5m --timeout=3s \
CMD curl -f http://localhost/ || exit 1
  

健康检查命令是：curl -f http://localhost/ || exit 1

两次检查的间隔时间是5秒

命令超时时间为3秒

