## Docker环境的安装与配置

本文目的是介绍如何在Ubuntu上安装Docker  
本文内容的实验环境为：Ubuntu14.04LTS（win7上用VMware运行的虚拟机）

### 1.本地安装docker

官方指南：[https://docs.docker.com/install/linux/docker-ce/ubuntu/](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

docker分为社区版（CE）和企业版（EE），其中前者免费使用而后者收费使用。

不管是CE版还是EE版，docker都只支持少数几种主流操作系统的主流版本，例如：Windows环境只支持Win10。

docker（CE）目前支持的操作系统如下图：![](/assets/docker001_002.png)

![](/assets/docker001_002.png)

STEP 0: 安装前检查  
docker一般只支持操作系统的最新几个主流版本，具体到Ubuntu，目前支持的包括：

### 2.安装管理工具

#### 2.1.本地安装docker-compose

#### 2.2.本地安装docker-machine

##### 创建一个本地docker machine

```shell
docker-machine create default
```

执行上面的命令可能会提示：  
（1）未安装virtualBox  
由于docker-machine使用与创建虚拟机，并在虚拟机上部署docker的工具，因此创建虚拟机需要安装虚拟机引擎。目前docker-machine支持多种虚拟机引擎（通过在create时输入的参数来配置），默认情况下选用virtualBox  
解决办法：安装virtualBox  
\`\`\`shell  
sudo apt-get update  
sudo apt-get install virtualbox

