## Python: 用C/C++扩展Python——模块（原创）

**参考资料**: PYTHON编程（第4版，下册）、Python核心编程（第3版）

### 1.C/C++扩展Python的一般编程范式

1.提供一个动态库，该动态库名称与Python中import的模块名一致，例如：如果希望在python中使用`import torch`来导入模块，那么该模块的动态库文件的名称应该是`torch.so`。类似的例子可以回想下`cv2.so`，遵循的是同样的规则。

2.动态库有一系列C/C++源码文件和头文件编程，其中有一个源文件中包含以下内容

* Part 1: 函数包装，用于在C/C++语言中，将函数输入参数和返回值在Python类型和C类型之间转化。
* Part 2: 方法定义，使用PyMethodDef定义一组模块方法。
* Part 3: 模块定义，使用PyModuleDef定义一个模块。
* Part 4: 模块导出，导出Python模块。

下面是大体源码结构

```cpp
#include <python3.6m/Python.h>

/* 第零部分：C/C++函数 */
/* 此部分可以在其他源文件中 */

/* 第一部分：函数包装 */
/* 注意：函数的返回值类型只能为void或PyObject *，形参类型只能是PyObject * */
static 返回值类型 函数名称(形参类型1 形参名1, 形参类型2 形参名2, ...)
{
    /* 将PyObject参数转换成C类型参数 */

    /* 函数操作 */

    /* 将结果包装成PyObject类型返回(如果有返回值的话) */
}

/* 第二部分: 方法定义 */
static PyMethodDef 方法定义数组名称[] = {
    { "Python中模块方法名称", 封装函数名称, METH_VARARGS, 方法说明 },

    // 若干格式相同的行...

    { NULL, NULL } // 结束行
};

/* 第三部分：模块定义 */
static struct PyModuleDef 模块定义变量名称 =
{
    PyModuleDef_HEAD_INIT,
    "cmathapi", // 对应于“模块名.__name__”, 这里的名字其实不重要
    // 填写其他字符串(与模块名称不一致)也不会影响编译好的动态库在使用时的模块名称
    "example module doc string", // 对应于“模块名.__doc__”, 也不是一个很重要的字段
    -1,
    MathMethods, // 方法定义数组名称
    NULL,
    NULL,
    NULL,
    NULL
};

/* 第四部分: 模块导出 */
PyMODINIT_FUNC PyInit_[python模块名]()
{
    PyObject* module = PyModule_Create(&模块定义变量名称);
    if (NULL == module) {
        return NULL;
    }

    /* 其他内容 */

    return module;
}
```

### 2.程序示例

#### 2.1 扩展部分的C++程序

* **说明**

增加了一个自定义的数学库模块，名称是cmathapi。该模块对外提供有两个方法，其一是两个整数的假发，另一个是两个整数的减法。

* **源码**

```cpp
/* test.cpp */
// 该头文件<python3.6m/Python.h>
// 一般位于 ananconda3/include下, 对应的动态库
#include <python3.6m/Python.h>
#include <iostream>

using namespace std;

int add(int arg1, int arg2)
{
    return arg1 + arg2;
}

int sub(int arg1, int arg2)
{
    return arg1 - arg2;
}

static PyObject* math_add(PyObject* self, PyObject* args)
{
    int arg1, arg2;
    if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2)) {
        return NULL;
    }
    int result = add(arg1, arg2);
    return (PyObject*)Py_BuildValue("i", result);
}

static PyObject* math_sub(PyObject* self, PyObject* args)
{
    int arg1, arg2;
    if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2)) {
        return NULL;
    }
    int result = sub(arg1, arg2);
    return (PyObject*)Py_BuildValue("i", result);
}

static PyMethodDef MathMethods[] = {
    { "add", math_add, METH_VARARGS, "Execute math command:add." },
    { "sub", math_sub, METH_VARARGS, "Execute math command:sub." },
    { NULL, NULL }
};

// 注意这里并不是定义了一个结构体类型, 而是定义并初始化了一个struct PyModuleDef类型的静态变量MathModule
static struct PyModuleDef MathModule =
{
    PyModuleDef_HEAD_INIT,
    "cmathapi", // 对应于“模块名.__name__”, 这里的名字似乎其实不重要
    // 填写其他字符串(与模块名称不一致)也不会影响编译好的动态库在使用时的模块名称
    "example module doc string", // 对应于“模块名.__doc__”, 也不是一个很重要的字段
    -1,
    MathMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

static PyObject* __mathError;

// 这里的函数名称似乎非常重要, 必须采用“pyInit_模块名”的模式命名
// 否则无法从编译得到的动态库中import指定模块名称
PyMODINIT_FUNC PyInit_cmathapi()
{
    PyObject* module = PyModule_Create(&MathModule);
    if (NULL == module) {
        return NULL;
    }
    __mathError = PyErr_NewException("math.error", NULL, NULL);
    Py_INCREF(__mathError);
    PyModule_AddObject(module, "error", __mathError);
    return module;
}
```

#### 2.2 扩展部分编译指令

```shell
#!/bin/sh
# command.sh

set -ex

if [ $# -nq 1 ]
then
  echo "legal number of params is 1, but now is $#"
  exit 1
else
  echo "ok, param number is $#"
fi

BUILD_COMMAND="g++ -Wall -g test.cpp -I $ANACONDA3_HOME/include  -Wl,-rpath,$ANACONDA3_HOME/lib -lpython3.6m -o Test"

LIB_COMMAND="g++ -fPIC -shared -Wall -g test.cpp -I $ANACONDA3_HOME/include -Wl,-rpath,$ANACONDA3_HOME/lib -lpython3.6m -o cmathapi.so"

if [ "$1" = build ]
then
  $BUILD_COMMAND
elif [ "$1" = lib ]
then
  $LIB_COMMAND
else
  echo "unknow command param $1, error"
  exit 1
fi
```

#### 2.3 Python测试代码

```python
#!coding=utf-8
#!/bin/env python

import cmathapi

def test(a, b):
    """hello"""
    c = cmathapi.add(a, b)
    print("{0} + {1} = {2}".format(a, b, c))

    d = cmathapi.sub(a, b)
    print("{0} - {1} = {2}".format(a, b, d))

if __name__ == '__main__':
    test(5, 7)
```

#### 2.4 使用方法

先将C++源码编译成`cmathapi.so`动态库，之后执行`python test.py`

```shell
$ sh command.sh
$ python test.py
5 + 7 = 12
5 - 7 = -2
```



