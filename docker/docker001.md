## Docker环境的安装与配置

本文介绍如何在Ubuntu上安装Docker  
本文所用的实验环境为：Ubuntu14.04LTS（win7上用VMware运行的虚拟机）

### 1.本地安装docker

官方指南：https://docs.docker.com/install/linux/docker-ce/ubuntu/

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

