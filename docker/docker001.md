## Docker环境的安装与配置

本文目的是介绍如何在Ubuntu上安装Docker  
本文内容的实验环境为：Ubuntu14.04LTS（win7上用VMware运行的虚拟机）

### 1.本地安装docker

官方指南：[https://docs.docker.com/install/linux/docker-ce/ubuntu/](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

docker分为社区版（CE）和企业版（EE），其中前者免费使用而后者收费使用。

不管是CE版还是EE版，docker都只支持少数几种主流操作系统的主流版本，例如：Windows环境只支持Win10。

docker（CE）目前支持的操作系统如下图：  
![](/assets/docker001_002.png)

**STEP 0: 安装前检查**  
docker一般只支持操作系统的最新几个主流版本，具体到Ubuntu，目前支持的包括：  
![](/assets/docker001_003.png)

除了操作系统外，还有一些其他要求，但一般的Ubuntu都能满足。

**STEP 1: 前驱安装**  
（1）安装依赖库

```shell
$ sudo apt-get update

$ sudo apt-get install \
    linux-image-extra-$(uname -r) \
    linux-image-extra-virtual
```

（2）Install packages to allow apt to use a repository over HTTPS:

```shell
$ sudo apt-get update

$ sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common
`
```

（3）Add Docker’s official GPG key:

```shell
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```

Verify that you now have the key with the fingerprint 9DC8 5822 9FC7 DD38 854A E2D8 8D81 803C 0EBF CD88, by searching for the last 8 characters of the fingerprint.

```shell
$ sudo apt-key fingerprint 0EBFCD88

pub   4096R/0EBFCD88 2017-02-22
      Key fingerprint = 9DC8 5822 9FC7 DD38 854A  E2D8 8D81 803C 0EBF CD88
uid                  Docker Release (CE deb) <docker@docker.com>
sub   4096R/F273FCD8 2017-02-22
```

（4）Use the following command to set up the stable repository.  
Note: lsb\_release -cs 返回Ubuntu的发行版本代号（对于14.04LTS来说就是trusty）.

```shell
$ sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
```

（5）检查安装

```shell
$ sudo docker version
```

### 2.设置不需要使用sudo即可调用docker命令

参考资料：[https://www.cnblogs.com/ksir16/p/6530587.html](https://www.cnblogs.com/ksir16/p/6530587.html)

* 添加一个用户组（下面用的用户组名字是docker）

```shell
$ sudo groupadd docker
```

* 将当前用户加入该group内，然后退出并重新登录生效。

```shell
$ sudo gpasswd -a ${USER} docker
```

* 重启docker服务

```shell
$ sudo service docker restart
```

* 新建一个shell，加入该用户组

```shell
$ ewgrp - docker
```

* 验证

```shell
$ docker run hello-world
```

### 3.安装管理工具

目前用到的docker管理工具有两个：docker-compose和docker-machine

#### 3.1.本地安装docker-compose

官方指南：[https://docs.docker.com/compose/install/](https://docs.docker.com/compose/install/)

* 必须确保已经在本地安装了docker。

* Run this command to download the latest version of Docker Compose（下载已经编译好的docker-compose的可执行文件）:

  ```shell
  sudo curl -L https://github.com/docker/compose/releases/download/1.21.2/docker-compose-$(uname -s)-$(uname -m) \
               -o /usr/local/bin/docker-compose
  ```

  注意，这里在实际操作时，由于实验环境的/usr/local/bin下存在旧版不支持HTTPS的curl，这里用sudo命令导致搜索可执行文件时未按照环境变量$PATH规定的顺序进行搜索，所以报错：

  ```shell
  curl: (1) Protocol "https" not supported or disabled in libcurl
  ```

  **解决办法：**  
  将解析uname命令返回值后的地址手动复制到浏览器下载  
  [https://github.com/docker/compose/releases/download/1.21.2/docker-compose-Linux-x86\_64](https://github.com/docker/compose/releases/download/1.21.2/docker-compose-Linux-x86_64手动复制到浏览器下载)  
  之后将下载到的文件docker-compose-Linux-x86\_64拷贝到目标目录，重命名为docker-compose

  ```shell
  sudo cp docker-compose-Linux-x86_64 /usr/local/bin/docker-compose
  ```

* Apply executable permissions to the binary:

  ```shell
  sudo chmod +x /usr/local/bin/docker-compose
  ```

* Test the installation.

  ```
  $ docker-compose --version
  docker-compose version 1.21.2, build 1719ceb
  ```

#### 3.2.本地安装docker-machine

官方指南：[https://docs.docker.com/machine/install-machine/](https://docs.docker.com/machine/install-machine/)

* 必须确保已经在本地安装了docker。

* 执行下列命令下载已经编译好的docker-machine的可执行文件

  ```shell
  $ base=https://github.com/docker/machine/releases/download/v0.14.0 &&
  curl -L $base/docker-machine-$(uname -s)-$(uname -m) >/tmp/docker-machine &&
  sudo install /tmp/docker-machine /usr/local/bin/docker-machine
  ```

  如果遇到无法下载的错误，类似于安装docker-compose的做法，可以直接把下载地址URL拷贝到浏览器下载，之后再改名复制到目标目录

* Check the installation by displaying the Machine version:

  ```shell
  $ docker-machine version
  docker-machine version 0.14.0, build 9371605
  ```

#### 3.3.创建一个本地docker machine

* 使用如下命令创建一个docker machine，下面的命令省略了一些参数（使用默认值）。

```shell
docker-machine create default
```

* 执行上面的命令可能会遇到如下问题：

  **问题1**：提示未安装virtualBox  
  由于docker-machine使用与创建虚拟机，并在虚拟机上部署docker的工具，因此创建虚拟机需要安装虚拟机引擎。目前docker-machine支持多种虚拟机引擎（通过在create时输入的参数来配置），默认情况下选用virtualBox  
  解决办法：安装virtualBox  
  \`\`\`shell  
  sudo apt-get update  
  sudo apt-get install virtualbox

  **问题2**：提示需要手动设置主板开启CPU功能 Intel VT-x 或 AMD-V  
  实验中遇到此问题是由于VMware虚拟机默认没有打开此项功能。打开此功能的办法：  
  （1）关闭VMware当前虚拟机  
  （2）在虚拟机窗口最上方一行中，依次点击“虚拟机” -&gt; “设置”，之后在弹出窗口的左侧选中“CPU”，在右侧的“虚拟化引擎”区域中勾选第二项“虚拟化 Intel VT-x/EPT或 AMD-V/RVI\(V\)”，如下图所示。

![](/assets/docker001_001.jpg)

（3）重启VMware虚拟机

* 查看当前所有machine
  ```shell
  docker-machine ls
  ```



