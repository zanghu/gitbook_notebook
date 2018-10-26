## VSCode编辑远程文件

官方资料：[Remote Workspace](https://marketplace.visualstudio.com/items?itemName=mkloubert.vscode-remote-workspace)

中文资料：[vscode-remote-workspace](https://davidham3.github.io/blog/2018/06/15/vscode-remote-workspace/)

实现使用本地IDE编辑远程服务器上的项目代码、利用远程服务器运行程序并将运行结果返回到本地IDE，这是程序开发者长久的追求。

本文作者尝试使用VSCode的远程工作区功能实现这一需求，取得了差强人意（勉强还算令人满意）的效果。

本文中本地IDE为windows下的VSCode，远程服务器是Ubuntu。

### 1.安装

#### 1.1.windows安装openSSH

从下面的超链接下载安装包后安装：

https://www.mls-software.com/files/setupssh-7.6p1-1.exe

安装完成后，打开一个命令行，输入ssh后回车检查是否安装成功。

#### 1.2.VSCode插件

VSCode需要安装插件Remote Workspace，安装方法是直接在查监控中搜索并安装。

### 2.配置工作区文本

VSCode将后缀名为`code-workspace`的文件作为工作区记录文件，VSCode可以通过读取该类型文件来打开一个工作区（展开工作区目录中所有文件）。

编辑一个文件"1.code-workspace"，包含以下内容：

```shell
// URL Format: sftp://[user:password@]host[:port][/path/to/a/folder][?param1=value1&param2=value2]
{
    "folders": [{
        "uri": "sftp://zanghu:zanghu@192.168.0.104/home/zanghu/code_box/caffe",
        "name": "My SFTP folder"
    }]
}
```

### 3.打开远程代码目录

VSCode中选择：文件 -> 打开工作区，选中“1.code-workspace”文件打开。

VSCode自动在左侧边栏中递归打开目录下所有文件，如下图所示：

![](/assets/others007_001.PNG)