## linkerd-examples: add-steps

### 一、运行程序

### 1.1.操作步骤

```shell
$ cd /path/to/linkerd-examples/add-steps 
```

### 1.2.安装

解压后将解药后的目录拷贝到希望安装的目录下，之后在~/.bashrc中配置环境变量
```shell
export MAVEN_HOME=$SOFTWARE_HOME/maven/maven-3.5.3 # SOFTWARE_HOME环境变量由用户自行设定为希望安装的目录
export PATH=$PATH:$MAVEN_HOME/bin
export MAVEN_OPT="-Xms64m -Xmx512m" # 可以不配置
```
在终端命令行执行
```shell
source ~/.bashrc
```

目录下内容如下图所示

![](/assets/linkerd001_001.png)
### 1.3.安装验证
终端命令行执行命令
```shell
mvn -v # 或者 mvn -version
```
显示maven版本信息则安装成功

### 二、Linux环境配置Maven

参考资料：http://wentao365.iteye.com/blog/903396/

### 2.1.配置settings.xml文件

由于需要配置仓库，因此目前的经验是maven不能离开服务器使用，必须将服务器信息配置在settings.xml中

例如，用户从maven安装目录的conf子目录下找到settings.xml，在settings.xml的&lt;mirrors&gt;&lt;/mirrors&gt;标签内加入如下图绿框中的内容

![](/assets/java002_001.png)

Linux环境下，maven会在用户首次调用maven创建项目时自动在用户主目录下生成.m2目录，将修改完成后的settings.xml文件拷贝到~/.m2目录下即可.