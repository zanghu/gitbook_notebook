## linkerd-examples: add-steps

参考资料：[https://github.com/linkerd/linkerd-examples/tree/master/add-steps](https://github.com/linkerd/linkerd-examples/tree/master/add-steps)

### 一、实验介绍

#### 1.1.实验目的

参考资料：[https://buoyant.io/2017/01/31/making-things-faster-by-adding-more-steps/](https://buoyant.io/2017/01/31/making-things-faster-by-adding-more-steps/)

In short, Linkerd’s latency-aware load balancing can rapidly detect when instances are slow and shift request traffic away from them. By combining this behavior with circuit breaking and retries, Linkerd can dramatically decrease end-to-end latency in a distributed system—despite being “yet another component”.

Let’s take a look at a dramatic example of this behavior with some poorly-behaved services.

#### 1.2.实验设计

* 集群A和B各包含10个独立服务器1-10，每个服务器上部署的都是相同的服务，人工设置节点延迟随着服务编号逐渐递增（0s-2s）；

* 两个发压客户端，每个客户端启动50个并发，一个客户端访问服务集群A（不使用linkerd），一个客户端访问服务集群B（使用linkerd），对比平均访问速度；
* 服务端程序使用Go语言编写，客_\*\*_户端使用bouyant公司开发的slow\_cooker（也是Go语言编写），性能数据收集和展示使用 prometheus + grafana

  实验内容示意图如下：

  ![](/assets/add-steps.png)

#### 1.3.所用工具

* **docker**：整个测试程序都是基于 docker 容器运行的。相关文件包括：Dockerfile、docker-compose.yml
* **bouyant linkerd**：service mesh 框架，测试主角兼被测对象；
* **bouyant slow_cooker**（慢炖锅）：发压客户端；
* **server.go**: 专门为本次测试编写的服务端程序（Go语言）；
* **prometeus**：监控数据收集工具，从服务端应用收集数据，使用Go语言开发。相关文件：prometeus.yml
* **Grafana**: 监控数据展示工具，负责展示（通过网页） prometheus 收集到的数据。相关文件：grafana.json，bootstrap.sh
 
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



