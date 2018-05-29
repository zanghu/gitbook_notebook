## Docker 常用操作——命令和参数

### 1.docker 命令：run

* **基于一个镜像启动一个容器**

```shell
$ docker run -ti ubuntu:latest echo hello world
```

* **基于一个镜像启动一个容器，后台运行**

```shell
$ docker run -ti -d ubuntu:latest echo hello world

```