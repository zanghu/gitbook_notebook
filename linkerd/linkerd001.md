## linkerd-examples: add-steps

### 二、运行程序

### 2.1.准备工作

* 下载 [https://github.com/linkerd/linkerd-examples/add-steps](https://github.com/linkerd/linkerd-examples/add-steps)
* 安装 docker
* 安装 docker-sompose 工具和 docker-machine 工具

### 2.2.启动服务容器

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
![](/assets/linkerd001_002.png)

### 2.3.启动展示工具

官方文档要求终端执行命令

```shell
$ open http://$(docker-machine ip default):3000 # or equivalent docker ip address
```

如果正常执行的话，会自动打开一个浏览器页面，展示性能检测结果，如下图所示  
![](/assets/linkerd001_006.PNG)

* **问题1**:

  **症状描述:**

  docker-machine 服务无法启动

  如果在执行此命令前没有创建默认的 docker-machine 服务，会报错  
    ![](/assets/linkerd001_003.PNG)

  **解决方法：**

  在命令行执行下面的命令创建默认 cocker-machine 服务

  ```shell
    $ docker-machine create default --driver virtualbox
  ```

  如果提示 virtualbox 不存在，ubuntu环境下可以使用下面的命令安装

  ```shell
    $ sudo-apt-get install virtualbox
  ```

* **问题2**

  **症状描述:**

  本地创建过 docker-machine 服务无法启动

  如果之前已经有默认的 docker-machine 服务，仍有可能报错：  
    ![](/assets/linkerd001_004.PNG)

  **解决方法**

  先删除旧的 docker-machine 服务，再重新创建

  ```shell
    $ docker-machine rm default
    $ docker-machine create default --driver virtualbox
  ```

* **问题3**

  **症状描述:**

  英文版报错：cannot get a file descriptor referring to the console，中文版则是：

  ![](/assets/linkerd001_005.PNG)

  原因应该是ubuntu下默认没有open程序

  解决办法  
    使用 xdg-open 代替 open，即执行如下命令：

  ```shell
    $ xdg-open http://$(docker-machine ip default):3000 # or equivalent docker ip address
  ```

* **问题4**

  **症状描述:**

  打开浏览器后页面无法正常显示。

  **解决办法**

  手动打开浏览器，在地址栏输入：

  ```
  http://localhost:3000/
  ```

  之后回车确定。



