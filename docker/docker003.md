## docker的一般概念与注意事项

### 1.关于Dockerfile

* 定义

Dockerfile是一个记录了构建目标镜像所必须的各种命令的文本文件。

docker可以通过读取一个Dockerfile文件中的配置信息快速自动创建一个镜像。

* 例子

下面的例子来源于: [https://github.com/linkerd/linkerd-examples/tree/master/add-steps](https://github.com/linkerd/linkerd-examples/tree/master/add-steps)

```shell
FROM golang:1.10.1-alpine3.7 # 创建目标镜像所需的基础镜像
WORKDIR /go/src/github.com/linkerd/linkerd-examples/add-steps/ # 为后续的其他指令（RUN、CMD、ENTRYPOINT）指明工作目录
RUN apk update && apk add git
RUN go get -d -v github.com/prometheus/client\_golang/prometheus
COPY server.go . # 从宿主机复制文件或目录到镜像内, 语法：COPY <src> <dst>，注意区分COPY与ADD的区别
RUN CGO\_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .

FROM scratch  
COPY --from=0 /go/src/github.com/linkerd/linkerd-examples/add-steps/app /app  
ENTRYPOINT \["/app"\]
```



