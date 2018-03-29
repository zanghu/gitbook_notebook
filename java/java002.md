## Linux环境配置Maven、创建HelloWorld项目

参考资料：http://wentao365.iteye.com/blog/903396/

### 1.配置settings.xml文件

由于需要配置仓库，因此目前的经验是maven不能离开服务器使用，必须将服务器信息配置在settings.xml中

Linux环境下，maven会在用户首次调用maven创建项目时自动在用户主目录下生成.m2目录，用户从maven安装目录的conf子目录下找到settings.xml, 修改完成后拷贝到~/.m2目录下即可.

例如，在settings.xml的&lt;mirrors&gt;&lt;/mirrors&gt;标签内加入如下图绿框中的内容

![](/assets/java002_001.png)

### 2.使用Maven创建HelloWorld工程

在命令行执行:

```shell
mvn -archetype:generate -DgroupId=com.abc.helloworld -DartifactId=helloworld_project
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

### 3.构建Maven项目

在命令行执行

```shell
cd [项目目录]
mvn package
```shell
执行完毕后，项目目录下会产生target目录，其下包括项目编译好的jar包, 包名是artifactId

### 4.执行项目
在命令行执行
```shell
$ java - cp target/helloworld_project-1.0-SNAPSHOT.jar com.abc.helloworld.App
Hello World!
```



