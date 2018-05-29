## Docker 常用操作——命令和参数

### 1.docker run 命令

* **基于一个镜像启动一个容器**

```shell
$ docker run -ti ubuntu:latest echo hello world
```

* **基于一个镜像启动一个容器，后台运行不会关闭**

```shell
$ docker run -ti -d ubuntu:latest echo hello world

```