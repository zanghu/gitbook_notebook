## DockerFile的常用命令

参考资料：[https://www.cnblogs.com/dazhoushuoceshi/p/7066041.html](https://www.cnblogs.com/dazhoushuoceshi/p/7066041.html)

### FROM

* **功能**

功能为指定基础镜像，并且必须是第一条指令。

* **语法**

```
FROM <image>  
FROM <image>:<tag>  
FROM <image>:<digest>
```

三种写法，其中&lt;tag&gt;和&lt;digest&gt; 是可选项，如果没有选择，那么默认值为latest

* **注意**

如果不以任何镜像为基础，那么写法为：

```
FROM scratch。
```

同时意味着接下来所写的指令将作为镜像的第一层开始

### RUN

* **功能**

功能为运行指定的命令

RUN命令有两种格式

```
# 第一种，后边直接跟shell命令。在linux操作系统上默认 /bin/sh -c，在windows操作系统上默认 cmd /S /C
RUN <command> 

# 第二种，类似于函数调用。可将executable理解成为可执行文件，后面就是两个参数。
RUN \["executable", "param1", "param2"\]
```

两种写法比对：

```
RUN /bin/bash -c 'source $HOME/.bashrc; echo $HOME  
RUN \["/bin/bash", "-c", "echo hello"\]
```

注意：多行命令不要写多个RUN，原因是Dockerfile中每一个指令都会建立一层.

多少个RUN就构建了多少层镜像，会造成镜像的臃肿、多层，不仅仅增加了构件部署的时间，还容易出错。

RUN书写时的换行符是\

### CMD

* **功能**

基于当前Dockerfile创建的镜像的容器启动时要运行的命令

* **语法**

语法有三种写法

```
# 语法1
CMD ["executable","param1","param2"]

# 语法2
CMD ["param1","param2"]

# 语法3
CMD command param1 param2
```

举例说明两种写法：

```
CMD [ "sh", "-c", "echo $HOME"  
CMD [ "echo", "$HOME" ]
```

补充细节：这里边包括参数的一定要用双引号，就是",不能是单引号。千万不能写成单引号。

* **注意**

RUN & CMD

不要把RUN和CMD搞混了。

RUN是构件容器时就运行的命令以及提交运行结果

CMD是容器启动时执行的命令，在构件时并不运行，构件时紧紧指定了这个命令到底是个什么样子

### LABEL

* **功能**

为镜像指定标签

* **语法**

语法：

```
LABEL <key>=<value> <key>=<value> <key>=<value> ...
```

一个Dockerfile种可以有多个LABEL，如下：

```
LABEL "com.example.vendor"="ACME Incorporated"  
LABEL com.example.label-with-value="foo"  
LABEL version="1.0"  
LABEL description="This text illustrates \  
that label-values can span multiple lines."
```

但是并不建议这样写，最好就写成一行，如太长需要换行的话则使用\符号，如下：

```
LABEL multi.label1="value1" \  
multi.label2="value2" \  
other="value3"
```

* **注意**

LABEL会继承基础镜像种的LABEL，如遇到key相同，则值覆盖

### MAINTAINER

* **功能**

指明Dockerfile的作者

* **语法**

```
MAINTAINER <name>
```

### EXPOSE

* **功能**

暴露容器运行时的监听端口给外部

但是EXPOSE并不会使容器访问主机的端口

如果想使得容器与主机的端口有映射关系，必须在容器启动的时候加上 -P参数

### ENV

* **功能**  

设置环境变量

* **语法**

语法有两种

```
# 语法1：一次设置一个
ENV <key> <value>

# 语法2：一次设置多个
ENV <key>=<value> ...
```

### ADD

* **功能**

一个复制命令，把文件复制到景象中。

如果把虚拟机与容器想象成两台linux服务器的话，那么这个命令就类似于scp，只是scp需要加用户名和密码的权限验证，而ADD不用。

* **语法**

语法如下：

```
# 语法1
ADD <src>... <dest>;

# 语法2
2. ADD ["<src>",... "<dest>"]
```

&lt;dest&gt;路径的填写可以是容器内的绝对路径，也可以是相对于工作目录的相对路径

&lt;src&gt;可以是一个本地文件或者是一个本地压缩文件，还可以是一个url

如果把&lt;src&gt;写成一个url，那么ADD就类似于wget命令

如以下写法都是可以的：

```
ADD test relativeDir/  
ADD test /relativeDir  
ADD http://example.com/foobar
```

* **注意**

尽量不要把&lt;scr&gt;写成一个文件夹，如果&lt;src&gt;是一个文件夹了，复制整个目录的内容,包括文件系统元数据

### COPY

看这个名字就知道，又是一个复制命令

语法如下：

1. COPY &lt;src&gt;... &lt;dest&gt;
2. COPY \["&lt;src&gt;",... "&lt;dest&gt;"\]
   与ADD的区别

COPY的&lt;src&gt;只能是本地文件，其他用法一致

### ENTRYPOINT

功能是启动时的默认命令

语法如下：

1. ENTRYPOINT \["executable", "param1", "param2"\]
2. ENTRYPOINT command param1 param2

如果从上到下看到这里的话，那么你应该对这两种语法很熟悉啦。

第二种就是写shell

第一种就是可执行文件加参数

与CMD比较说明（这俩命令太像了，而且还可以配合使用）：

相同点：

只能写一条，如果写了多条，那么只有最后一条生效

容器启动时才运行，运行时机相同

不同点：

ENTRYPOINT不会被运行的command覆盖，而CMD则会被覆盖

如果我们在Dockerfile种同时写了ENTRYPOINT和CMD，并且CMD指令不是一个完整的可执行命令，那么CMD指定的内容将会作为ENTRYPOINT的参数

如下：

FROM ubuntu  
ENTRYPOINT \["top", "-b"\]  
CMD \["-c"\]  
如果我们在Dockerfile种同时写了ENTRYPOINT和CMD，并且CMD是一个完整的指令，那么它们两个会互相覆盖，谁在最后谁生效

如下：

FROM ubuntu  
ENTRYPOINT \["top", "-b"\]  
CMD ls -al  
那么将执行ls -al ,top -b不会执行。

