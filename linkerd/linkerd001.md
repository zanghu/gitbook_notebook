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



### 二、Linux环境配置Maven

参考资料：http://wentao365.iteye.com/blog/903396/

### 2.1.配置settings.xml文件

由于需要配置仓库，因此目前的经验是maven不能离开服务器使用，必须将服务器信息配置在settings.xml中

例如，用户从maven安装目录的conf子目录下找到settings.xml，在settings.xml的&lt;mirrors&gt;&lt;/mirrors&gt;标签内加入如下图绿框中的内容

![](/assets/java002_001.png)

Linux环境下，maven会在用户首次调用maven创建项目时自动在用户主目录下生成.m2目录，将修改完成后的settings.xml文件拷贝到~/.m2目录下即可.