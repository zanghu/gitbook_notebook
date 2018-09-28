## 配置文件 docker-compose.yml 的例子

* **示例来源**

先来看一个例子，来源于：[https://github.com/linkerd/linkerd-examples/tree/master/add-steps](https://github.com/linkerd/linkerd-examples/tree/master/add-steps)，这里加入了一些注释

例子的原始目的是要在服务集群各节点连接速度不稳定的情况下，在使用linkerd和不使用linkerd两种情况下，对比客户端调用服务的速度。

* **实验设计**

* 集群A和B各包含10个独立服务器1-10，每个服务器上部署的都是相同的服务，人工设置节点延迟随着服务编号逐渐递增（0s-2s）；

* 两个发压客户端，每个客户端启动50个并发，一个客户端访问服务集群A（不使用linkerd），一个客户端访问服务集群B（使用linkerd），对比平均访问速度；
* 服务端程序使用Go语言编写，客_\*\*_户端使用bouyant公司开发的slow\_cooker（也是Go语言编写），性能数据收集和展示使用 prometheus + grafana

实验内容示意图如下：

![](/assets/add-steps.png)

* **docker-compose.yml 内容分析**

```
# docker-compose 语法版本，目前最高是3
version: '2'

# 构成应用的所有服务，每个服务启动一个容器，每个子段落对应于一个容器的配置
# 
services:

# 配置10个不使用linkerd连接的服务的容器
# 前8个服务只配置延迟，最后两个还有一定概率失败（返回HTTP错误状态码）
  baseline_app1: # 容器名称
    build: . # 容器对应的镜像的Dokerfile文件所在的目录
    networks: # 网络配置
      default: # 使用默认网络
        aliases: # 为当前网络起一个别名
         - baseline_app

  baseline_app2:
    build: .
    command: -latency=10ms # 覆盖镜像的Dockerfile的CMD命令
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

# 配置10个使用linkerd调用的服务的容器
# 前8个服务只配置延迟，最后两个还有一定概率失败（返回HTTP错误状态码）

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

  # 配置linkerd服务的容器
  linkerd:
    image: buoyantio/linkerd:1.3.6
    ports: # 容器端口与宿主机端口的映射关系
      - 4140:4140 # linkerd对客户端暴露的服务访问端口
      - 9990:9990 # linkerd的控制面板访问端口
    volumes: # 数据卷, docker run -v
      - ./linkerd.yml:/io/buoyant/linkerd/config.yml:ro # 第二个冒号后main的ro表示以只读模式加载数据卷
      - ./disco:/disco
    command:
      - "/io/buoyant/linkerd/config.yml"

  # 配置性能数据收集工具的服务容器
  prometheus:
    image: prom/prometheus:v1.4.1
    volumes:
      - ./prometheus.yml:/prometheus/prometheus.yml:ro
    command: >
      -config.file=/prometheus/prometheus.yml
      -storage.local.retention=12h
    ports:
      - 9090:9090

  # 配置性能数据展示工具的服务容器
  grafana:
    build:
      context: .
      dockerfile: Dockerfile-grafana
    ports:
      - 3000:3000
    environment:
      - GF_AUTH_ANONYMOUS_ENABLED=true
      - GF_AUTH_ANONYMOUS_ORG_ROLE=Admin

  # 配置不使用linkerd的发压客户端的容器
  baseline_slow_cooker:
    image: buoyantio/slow_cooker:1.1.0-golang1.8rc2
    entrypoint: /bin/sh
    command: >
      -c 'sleep 15 && slow_cooker -noreuse -metric-addr :8505 -qps 10 -concurrency 50 -interval 5s -totalRequests 10000000 http://baseline_app:8501'

  # 配置使用linkerd的发压客户端的容器
  linkerd_slow_cooker:
    image: buoyantio/slow_cooker:1.1.0-golang1.8rc2
    entrypoint: /bin/sh
    command: >
      -c 'sleep 15 && slow_cooker -noreuse -metric-addr :8505 -qps 10 -concurrency 50 -interval 5s -totalRequests 10000000 http://linkerd:4140'
```



