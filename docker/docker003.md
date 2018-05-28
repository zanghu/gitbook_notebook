## docker的一般概念与注意事项

### 1.关于Dockerfile

* 定义

Dockerfile是一个记录了构建目标镜像所必须的各种命令的文本文件。

docker可以通过读取一个Dockerfile文件中的配置信息快速自动创建一个镜像。

* 例子

下面的例子来源于: [https://github.com/linkerd/linkerd-examples/tree/master/add-steps](https://github.com/linkerd/linkerd-examples/tree/master/add-steps)

```shell
# 创建目标镜像所需的基础镜像，并且必须是第一条指令。
FROM golang:1.10.1-alpine3.7

# 为后续的其他指令（RUN、CMD、ENTRYPOINT）指明工作目录
WORKDIR /go/src/github.com/linkerd/linkerd-examples/add-steps/

# 安装一些必要的软件
RUN apk update && apk add git
RUN go get -d -v github.com/prometheus/client\_golang/prometheus

# 从宿主机复制文件或目录到镜像内, 语法：COPY <src> <dst>，注意区分COPY与ADD的区别
COPY server.go . 
RUN CGO\_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .

# 如果不以任何镜像为基础，那么写法为：FROM scratch
FROM scratch  
COPY --from=0 /go/src/github.com/linkerd/linkerd-examples/add-steps/app /app

# 基于镜像启动容器后首先执行的命令，注意与RUN和CMD区分
ENTRYPOINT \["/app"\]
```

RUN 与 CMD和ENTRYPOINT 的区别
RUN后面的内容是用来创建镜像时执行的命令。
CMD和ENTRYPOINT后面的内容是基于镜像的容器在启动后自动执行的命令。

CMD 与ENTRYPOINT的区别
1. 相同点：
只能写一条，如果写了多条，那么只有最后一条生效
容器启动时才运行，运行时机相同

2. 不同点：
ENTRYPOINT不会被运行的command覆盖，而CMD则会被覆盖
如果在Dockerfile中同时写了ENTRYPOINT和CMD，并且CMD指令不是一个完整的可执行命令，那么CMD指定的内容将会作为ENTRYPOINT的参数



