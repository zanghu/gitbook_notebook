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

### 2.使用 dokcer-compose 

docker-compose的本质，就是通过一个配置文件将多个咋的docker run指令统合在一起，按序执行

因此，docker-compose.yml中的配置的内容从根本上说，就是每个 docker run 命令的参数。

三个基本步骤：

* Step 1: 在 DockerFile 文件中定义你的应用的环境变量，这样应用即可在任何地方被复现.

* Step 2: 在 docker-compose.yml 文件中定义构成你的应用的全部服务，这样它们就可以在一个单独的环境上一起运行.

* Step 3: 执行 docker-compose up 命令，之后 docker-compose 就会启动和运行你的整个应用.

### 3.配置文件 docker-compose.yml 的例子

先来看一个例子，来源于：[https://github.com/linkerd/linkerd-examples/tree/master/add-steps](https://github.com/linkerd/linkerd-examples/tree/master/add-steps)，这里加入了一些注释

```shell
# docker-compose 语法版本，目前最高是3
version: '2'

# 构成应用的所有服务，每个服务启动一个容器，每个子段落对应于一个容器的配置
# 
services:

  baseline_app1:
    build: .
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app2:
    build: .
    command: -latency=10ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app3:
    build: .
    command: -latency=20ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app4:
    build: .
    command: -latency=40ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app5:
    build: .
    command: -latency=80ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app6:
    build: .
    command: -latency=160ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app7:
    build: .
    command: -latency=320ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app8:
    build: .
    command: -latency=640ms
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app9:
    build: .
    command: -latency=1s -success-rate=0.6
    networks:
      default:
        aliases:
         - baseline_app

  baseline_app10:
    build: .
    command: -latency=2s -success-rate=0.4
    networks:
      default:
        aliases:
         - baseline_app

  linkerd_app1:
    build: .

  linkerd_app2:
    build: .
    command: -latency=10ms

  linkerd_app3:
    build: .
    command: -latency=20ms

  linkerd_app4:
    build: .
    command: -latency=40ms

  linkerd_app5:
    build: .
    command: -latency=80ms

  linkerd_app6:
    build: .
    command: -latency=160ms

  linkerd_app7:
    build: .
    command: -latency=320ms

  linkerd_app8:
    build: .
    command: -latency=640ms

  linkerd_app9:
    build: .
    command: -latency=1s -success-rate=0.6

  linkerd_app10:
    build: .
    command: -latency=2s -success-rate=0.4

  linkerd:
    image: buoyantio/linkerd:1.3.6
    ports:
      - 4140:4140
      - 9990:9990
    volumes:
      - ./linkerd.yml:/io/buoyant/linkerd/config.yml:ro
      - ./disco:/disco
    command:
      - "/io/buoyant/linkerd/config.yml"

  prometheus:
    image: prom/prometheus:v1.4.1
    volumes:
      - ./prometheus.yml:/prometheus/prometheus.yml:ro
    command: >
      -config.file=/prometheus/prometheus.yml
      -storage.local.retention=12h
    ports:
      - 9090:9090

  grafana:
    build:
      context: .
      dockerfile: Dockerfile-grafana
    ports:
      - 3000:3000
    environment:
      - GF_AUTH_ANONYMOUS_ENABLED=true
      - GF_AUTH_ANONYMOUS_ORG_ROLE=Admin

  baseline_slow_cooker:
    image: buoyantio/slow_cooker:1.1.0-golang1.8rc2
    entrypoint: /bin/sh
    command: >
      -c 'sleep 15 && slow_cooker -noreuse -metric-addr :8505 -qps 10 -concurrency 50 -interval 5s -totalRequests 10000000 http://baseline_app:8501'

  linkerd_slow_cooker:
    image: buoyantio/slow_cooker:1.1.0-golang1.8rc2
    entrypoint: /bin/sh
    command: >
      -c 'sleep 15 && slow_cooker -noreuse -metric-addr :8505 -qps 10 -concurrency 50 -interval 5s -totalRequests 10000000 http://linkerd:4140'

```

