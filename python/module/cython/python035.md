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

```cython
# cython_hello_world.pyx
# cython: language_level=3

cdef extern from "hello_world.h":
    void print_hello_world()

def hello_world():
    print_hello_world()
```