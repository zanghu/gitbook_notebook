## 通过cython调用C库的入门例子

参考资料：[简书：用Cython加速Python到“起飞”](https://www.jianshu.com/p/fc5025094912)

参考资料：[GitHub: hello_world](https://github.com/yeyuzhen/EasonCodeShare/tree/master/cython_tutorials/hello_world)

注意：将**cython**与**CPython**区分！

### 1. 源码

C头文件：`hello_world.h`

```c
/* hello_world.h */
#pragma once
void print_hello_world();
```

C源文件：`hello_world.c`

```c
/* hello_world.c */
#include <stdio.h>
#include "hello_world.h"

void print_hello_world()
{
    printf("hello world!\n");
}
```

cython文件：`cython_hello_world.pyx`
注意必须加入`# cython: language_level=3`，否则可能按照python2方式转译

```python
# cython_hello_world.pyx
# cython: language_level=3

cdef extern from "hello_world.h":
    void print_hello_world()

def hello_world():
    print_hello_world()
```

编译脚本：`build.sh`
> 注意：生成的动态库的名字（其实是动态库名字中第一个'.'之前的部分）必须与*.c源文件名严格相同，否则pythoh加载会失败

```shell
#!/bin/bash

set -ex

# 生成cython_hello_world.c文件
cython cython_hello_world.pyx

# *.c -> *.o
gcc -g -Wall -c -fPIC -I/home/zanghu/ProgramFiles/software/anaconda3/2019.03/include/python3.7m *.c 

# *.o -> cython_hello_world.so
# 注意：生成的动态库的名字（其实是动态库名字中第一个'.'之前的部分）必须与*.c源文件名严格相同，否则pythoh加载会失败
gcc -g -Wall -shared *.o -o cython_hello_world.so
```

### 2. 运行

```shell
$ sh build.sh
$ python
>>> import cython_hello_world
>>> cython_hello_world.hello_world()
hello world!
>>>
```