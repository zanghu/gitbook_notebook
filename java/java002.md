## Linux环境配置Maven、创建HelloWorld项目

参考资料：[http://wentao365.iteye.com/blog/903396/](http://wentao365.iteye.com/blog/903396/)

### 1.使用Maven创建HelloWorld工程

在命令行执行:

```shell
mvn archetype:generate -DgroupId=com.abc.helloworld -DartifactId=helloworld_project
```

其中：-DfroupId指定的是创建的项目的包路径（构建后可执行文件的调用全名前缀），-DartifactId指定的是创建的项目的顶级目录名（也是构建后的jar包名）  
初次执行maven，会从服务器下载一些数据。执行完成后，会自动在当前目录下创建helloworld\_project目录，其中的结构包括:

```shell
helloworld\_project  
    \|--pom.xml  
    \|--src  
        \|--main  
            \|--java  
                \|--com   
                    \|--abc  
                        \|--helloworld  
                            \|--AppTest.java  
        \|--text  
            \|--java  
                \|--com   
                    \|--abc  
                        \|--helloworld  
                            \|--App.java
```

可见maven项目的基本目录结构是:

```shell
project_name  
    \|--pom.xml  
    \|--src  
        \|--main   
        \|--text  
    \|--target
```

### 2.构建Maven项目

在命令行执行

```shell
cd [项目目录]
mvn package
```shell
执行完毕后，项目目录下会产生target目录，其下包括项目编译好的jar包, 包名是artifactId

# 执行项目
```shell
$ java -cp target/helloworld_project-1.0-SNAPSHOT.jar com.abc.helloworld.App
Hello World!
```

### 3.配置依赖jar包

maven是JAVA项目构建框架，同时也具备项目依赖库（jar包）版本管理功能。

为maven项目配置依赖jar包的方法之一是：将以来的jar包安装到本机maven软件版本库，方法如下：

```shell
# 假设要安装的jar包名为: netty-all-4.1.22.Final.jar
mvn install:install-file -Dfile=/path/to/netty-all-4.1.22.Final.jar \
                         -DgroupId=io.netty \
                         -DartifactId=netty-all \
                         -Dversion=4.1.22-Final \
                         -Dpackaging=jar
```
注意：上面的命令中的参数-Dfile、-DgroupId、-DartifactId、-Dversion、-Dpackaging不可缺少，否则会报错。

之后，在maven项目的目录下的pol.xml文件中配置



