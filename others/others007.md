## VSCode编辑远程文件

实现使用本地IDE编辑远程服务器上的项目代码、利用远程服务器运行程序并将运行结果返回到本地IDE，这是程序开发者长久的追求。

本文作者尝试使用VSCode的编辑远程文件功能实现这一需求，虽然最终效果不令人满意，但是仍有记录下来的价值。

本文中本地IDE为windows下的VSCode，远程服务器是Ubuntu。

### 1.安装

#### 1.1.windows安装openSSH

从下面的超链接下载安装包后安装：

https://www.mls-software.com/files/setupssh-7.6p1-1.exe

安装完成后，打开一个命令行，输入ssh后回车检查是否安装成功。

#### 1.2.VSCode插件

VSCode需要安装插件Remote VSCode，安装方法是直接在查监控中搜索并安装。

#### 1.3.服务端安装rmate

首先，安装gem

```shell
$ sudo apt-get install gem
```

之后，使用gem安装rmate

```shell
$ gem install rmate
```

### 2.使用VSCode修改远程文件

#### 2.1.VSCode终端远程连接远程服务器

在VSCode中: 查看 -> 终端，之后在窗口底部出现的终端中输入：ssh -R 52698:127.0.0.1:52698 远程服务器用户名@远程服务器地址

```shell
$ ssh -R 52698:127.0.0.1:52698 zanghu@192.168.0.104
```

#### 2.2.使用rmate打开远程文件

继续在VSCode终端中输入下面的命令：

```shell
$ rmate -p 52698 /home/zanghu/record
```

VSCode编辑窗中会打开该远程文件。
