## 通过cython调用C库的入门例子


```c
/* hello_world.h */
#pragma once
void print_hello_world();
```

```c
/* hello_world.c */
#include <stdio.h>
#include "hello_world.h"

void print_hello_world()
{
    printf("hello world!\n");
}
```

```python
# cython_hello_world.pyx
# cython: language_level=3

cdef extern from "hello_world.h":
    void print_hello_world()

def hello_world():
    print_hello_world()
```

```shell
# build.sh
#!/bin/bash

set -ex

cython cython_hello_world.pyx
gcc -g -Wall -c -fPIC -I/home/zanghu/ProgramFiles/software/anaconda3/2019.03/include/python3.7m *.c 
gcc -g -Wall -shared *.o -o cython_hello_world.so
```