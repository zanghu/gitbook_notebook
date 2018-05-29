## Docker 常用操作——命令和参数

### 1.容器运行命令：run

* **基于一个镜像启动一个容器**

```shell
$ docker run -ti ubuntu:latest echo hello world
```

* **基于一个镜像启动一个容器，后台运行**

```shell
# 增加的-d 参数使得容器在后台运行，运行结果不会被打印.
# 容器启动后会返回一个容器ID.
$ docker run -ti -d ubuntu:latest echo hello world
7b6c6037b2a4de816cc244b85112605936aa5fb69608ad9fa17330b451a743ce
```

### 2.容器查看命令：ps



### 3.镜像查看命令：inspect

```shell
$ docker inspect ubuntu: latest
```