## linkerd-examples: add-steps

### 一、运行程序

### 1.1.准备工作

* 下载 https://github.com/linkerd/linkerd-examples/add-steps
* 安装 docker
* 安装 docker-sompose 工具和 docker-machine 工具

### 1.2.启动服务容器

```shell
$ cd /path/to/linkerd-examples/add-steps 
```

目录下内容如下图所示：
![](/assets/linkerd001_001.png)

使用 docker-compose 工具，先用build实现容器进行构建，再通过up来启动容器：

```shell
$ docker-compose build && docker-compose up -d
```

执行后效果如下图:
![](/assets/linkerd_001_002.png)

### 1.3.启动展示工具

终端执行命令

```shell
$ open http://$(docker-machine ip default):3000 # or equivalent docker ip address
```

如果在执行此命令前没有创建默认的 docker-machine 服务，会报错
![](/assets/linkerd001_003.PNG)



