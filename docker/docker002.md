## 使用工具： docker-compose

参考资料1：[https://docs.docker.com/compose/overview/](https://docs.docker.com/compose/overview/)

参考资料2：[https://docs.docker.com/compose/gettingstarted/](https://docs.docker.com/compose/gettingstarted/)

### 1.为什么需要docker-compose

（1）docker容器运行的一般方式:

* **STEP 1**: 创建镜像; 如果运行容器所用的所有镜像可以直接从仓库下载或已在本地，则可以省略此步

* **STEP 2**: 启动容器; 一般使用 docker run -ti [参数]

（2）按照上述方法运行docker容器，存在如下缺点:

* **缺点1**: 如果每个容器的配置参数很多，那么每个 docker run 命令会非常复杂，且这种“命令 + 参数”的模式会使得配置参数难以阅读

* **缺点2**: 如果同时要启动多个容器的场景（比如linkerd的例子, add-steps中，需要启动多个服务外（每个服务独占一个容器）加linkerd和配套性能监控），就需要多次执行 docker run 命令，操作过程繁琐且可读性差。

（3）为解决上述问题，docker提供了一个管理复杂docker容器任务（主要面向多容器管理的场景）的工具: docker-compose

按照官方文档**定义**:
```
Compose is a tool for defining and running multi-container Docker applications.
```

### 2.使用 dokcer-compose 的三个基本步骤：

* Step 1: 在 DockerFile 文件中定义你的应用的环境变量，这样应用即可在任何地方被复现.

* Step 2: 在 docker-compose.yml 文件中定义构成你的应用的服务，这样它们就可以在一个孤立环境上一起运行.

* Step 3: 执行 docker-compose up 命令，之后 Compose 就会启动和运行你的整个应用.



