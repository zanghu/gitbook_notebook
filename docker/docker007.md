## Docker 常用操作——命令和参数

### 1.容器运行命令：run

* **基于一个镜像启动一个容器**

```shell
$ docker run -ti ubuntu:latest echo hello world
```

* **基于一个镜像启动一个容器，后台运行**

```shell
# 增加的-d 参数使得容器在后台运行，运行结果不会被打印.
# 容器启动后会返回一个容器ID.
$ docker run -ti -d ubuntu:latest echo hello world
7b6c6037b2a4de816cc244b85112605936aa5fb69608ad9fa17330b451a743ce
```

### 2.容器查看命令：ps



### 3.镜像查看命令：inspect

```shell
$ docker inspect ubuntu:latest
```
# 以下为返回内容
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