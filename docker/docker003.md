## docker的一般概念与注意事项

### 1.关于Dockerfile

* 定义

Dockerfile是一个记录了构建目标镜像所必须的各种命令的文本文件。

docker可以通过读取一个Dockerfile文件中的配置信息快速自动创建一个镜像。

* 例子
下面的例子来源于: 
```shell
FROM golang:1.10.1-alpine3.7
WORKDIR /go/src/github.com/linkerd/linkerd-examples/add-steps/
RUN apk update && apk add git
RUN go get -d -v github.com/prometheus/client_golang/prometheus
COPY server.go .
RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .

FROM scratch
COPY --from=0 /go/src/github.com/linkerd/linkerd-examples/add-steps/app /app
ENTRYPOINT ["/app"]

```








