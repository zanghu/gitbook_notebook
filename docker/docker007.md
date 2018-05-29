## Docker 常用操作——命令和参数

### 1.容器运行命令：run

* **最常用的参数**

```
-i, --interactive=false   打开STDIN，用于控制台交互    
-t, --tty=false            分配tty设备，该可以支持终端登录，默认为false    
```

* **基于一个镜像启动一个容器**

```shell
$ docker run -ti ubuntu:latest echo hello world
```

* **基于一个镜像启动一个容器，后台运行**

```shell
# 增加的-d 参数使得容器在后台运行，运行结果不会被打印.
# 容器启动后会返回一个容器ID.
$ docker run -ti -d ubuntu:latest echo hello world
```

* **覆盖镜像默认的CMD命令：--entrypoint**

  很多镜像在创建时都指定了实例化为容器后的默认动作，Dockerfile 创建镜像时，CMD命令就是用来指定镜像实例化后的默认命令的。

  某些情况下，不希望这些默认命令被启动，比如希望在容器启动后研究下容器内部某个文件的内容、文件组织等等。

  这时，在 docker run 中加入 --entrypoint 参数可以覆盖原镜像的默认启动命令。

  例如，希望在启动容器后，显示容器内的目录结构

  ```shell
  $ docker run -ti --entrypoint ls grafana/grafana:3.1.1 -al
  ```
  
  这里注意 --entrypoit的命令如果有参数，那么一定要放在镜像名之后，像下面这样的命令会报错。
  
  ```shell
  $ docker run -ti --entrypoint "ls -al" grafana/grafana:3.1.1 # 是错误的
  ```
  
  参考资料：[https://medium.com/@oprearocks/how-to-properly-override-the-entrypoint-using-docker-run-2e081e5feb9d](https://medium.com/@oprearocks/how-to-properly-override-the-entrypoint-using-docker-run-2e081e5feb9d)

* **其他常用参数**

```
-w, --workdir=""           指定容器的工作目录  
-c, --cpu-shares=0        设置容器CPU权重，在CPU共享场景使用
-e, --env=[]               指定环境变量，容器中可以使用该环境变量
-h, --hostname=""          指定容器的主机名
--net="bridge"             容器网络设置
-v, --volume=[]            给容器挂载存储卷，挂载到容器的某个目录
--volumes-from=[]          给容器挂载其他容器上的卷，挂载到容器的某个目录
```

  返回容器ID
  
```shell
7b6c6037b2a4de816cc244b85112605936aa5fb69608ad9fa17330b451a743ce
```

### 2.容器查看命令：ps

* 查看所有容器

```shell
$ docker ps -a
```

* 查看正在运行的容器

```shell
$ docker ps
```

* 查看容器占用内存大小

```shell
$ docker ps -s
```

### 3.连接后台运行的容器：attach

启动一个容器，后台运行

```shell
$ docker run -tid ubuntu:14.04 /bin/sh # 可以通过 docker ps 检查后台容器是否启动，并获得容器ID
```

连接这个容器
```shell
$ docker attach badf191e93f5
```

### 4.启动容器控制台：exec

要想研究一个容器的内部，最简单的想法就是在在容器内打开一个终端，之后通过终端查看容器内文件系统.

实现方法：

首先使用 docker ps 查看当前正在运行的所有容器，找到目标容器的id

之后，使用 docker exec 打开容器内终端

```shell
$ docker exec [options] [容器id]
```

举个例子

```shell
$ docker run -tid ubuntu:14.04 /bin/sh
c45d581ad4da
$ docker exec -ti c45d581ad4da /bin/sh
#
```

这个方法很适合那些存在默认启动命令、并且启动命令会妨碍终端操作的容器.

### 4.镜像查看命令: images

列出本地镜像。

```shell
docker images [OPTIONS] [REPOSITORY[:TAG]]
```

OPTIONS说明：

```
-a :列出本地所有的镜像（含中间映像层，默认情况下，过滤掉中间映像层）；
--digests :显示镜像的摘要信息；
-f :显示满足条件的镜像；
--format :指定返回值的模板文件；
--no-trunc :显示完整的镜像信息；
-q :只显示镜像ID。
```

### 5.镜像搜索命令：search

从Docker Hub查找镜像

```shell
$ docker search [OPTIONS] TERM
```

OPTIONS说明：

```
--automated :只列出 automated build类型的镜像；
--no-trunc :显示完整的镜像描述；
-s :列出收藏数不小于指定值的镜像。
```

### 6.镜像查看命令：inspect

查看指定镜像的详细信息。

如果把镜像都看作是由 Dockerfile 创建的，那么 inspec 相当于查看创建镜像的 Dockerfile 的内容。

```shell
$ docker inspect ubuntu:latest
```

以下为返回内容
```shell
[
    {
        "Id": "sha256:452a96d81c30a1e426bc250428263ac9ca3f47c9bf086f876d11cb39cf57aeec",
        "RepoTags": [
            "ubuntu:latest"
        ],
        "RepoDigests": [
            "ubuntu@sha256:c8c275751219dadad8fa56b3ac41ca6cb22219ff117ca98fe82b42f24e1ba64e"
        ],
        "Parent": "",
        "Comment": "",
        "Created": "2018-04-27T23:28:36.319694807Z",
        "Container": "e1a8ac8f61e4bd40dd223471e86b0328609182a3112dd45a435575753bbc7924",
        "ContainerConfig": {
            "Hostname": "e1a8ac8f61e4",
            "Domainname": "",
            "User": "",
            "AttachStdin": false,
            "AttachStdout": false,
            "AttachStderr": false,
            "Tty": false,
            "OpenStdin": false,
            "StdinOnce": false,
            "Env": [
                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
            ],
            "Cmd": [
                "/bin/sh",
                "-c",
                "#(nop) ",
                "CMD [\"/bin/bash\"]"
            ],
            "ArgsEscaped": true,
            "Image": "sha256:a43f69020c4ca7feb3cfb5fa5857a24b138efa953f6108975205c1f121c7c9cb",
            "Volumes": null,
            "WorkingDir": "",
            "Entrypoint": null,
            "OnBuild": null,
            "Labels": {}
        },
        "DockerVersion": "17.06.2-ce",
        "Author": "",
        "Config": {
            "Hostname": "",
            "Domainname": "",
            "User": "",
            "AttachStdin": false,
            "AttachStdout": false,
            "AttachStderr": false,
            "Tty": false,
            "OpenStdin": false,
            "StdinOnce": false,
            "Env": [
                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
            ],
            "Cmd": [
                "/bin/bash"
            ],
            "ArgsEscaped": true,
            "Image": "sha256:a43f69020c4ca7feb3cfb5fa5857a24b138efa953f6108975205c1f121c7c9cb",
            "Volumes": null,
            "WorkingDir": "",
            "Entrypoint": null,
            "OnBuild": null,
            "Labels": null
        },
        "Architecture": "amd64",
        "Os": "linux",
        "Size": 79620962,
        "VirtualSize": 79620962,
        "GraphDriver": {
            "Data": null,
            "Name": "aufs"
        },
        "RootFS": {
            "Type": "layers",
            "Layers": [
                "sha256:65bdd50ee76a485049a2d3c2e92438ac379348e7b576783669dac6f604f6241b",
                "sha256:ec75999a0cb1218bbfedeaf535afb516b739c0a2475f89d6c8bdf6ccfdf73c85",
                "sha256:67885e448177114ca1d82161955ba7400139b5acc1e9cca633dfff163ccdb1b6",
                "sha256:8db5f072feeccc451f94b357a1f596dd455859807917e7c98ed7264601043cbf",
                "sha256:059ad60bcacfe9c03116f467f9e025f3519d1641358e0422bee478445d679313"
            ]
        },
        "Metadata": {
            "LastTagTime": "0001-01-01T00:00:00Z"
        }
    }
]
```








