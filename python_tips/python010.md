## Python: 为什么函数默认参数不应该是list类型

参考资料: [http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html](http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html)

### 1.C/C++扩展Python的一般编程范式

1.提供一个动态库，该动态库名称与Python中import的模块名一致，例如：如果希望在python中使用`import torch`来导入模块，那么该模块的动态库文件的名称应该是`torch.so`。类似的例子可以回想下`cv2.so`，遵循的是同样的规则。

2.动态库有一系列C/C++源码文件和头文件编程，其中有一个源文件中包含以下内容

```cpp
#include <python3.6m/Python.h>

/* 第一部分：方法封装 */
static 返回值类型 函数名称(形参类型1 形参名1, 形参类型2 形参名2, ...)
{
    /* 将PyObject参数转换成C类型参数 */
    
    /* 函数操作 */
    
    /* 将结果包装成PyObject类型返回(如果有返回值的话) */
}

```

### 2.扩展部分的C++程序

* **说明**

增加了一个自定义的数学库模块，名称是cmathapi。该模块对外提供有两个方法，其一是两个整数的假发，另一个是两个整数的减法。

* **源码**

```cpp
// 该头文件<python3.6m/Python.h>
一般位于 ananconda3/include下, 对应的动态库
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
 
static struct PyModuleDef MathModule =
{
    PyModuleDef_HEAD_INIT,
    "cmathapi", // 对应于“模块名.__name__”, 这里的名字似乎其实不重要, 填写其他字符串(与模块名称不一致)也不会影响编译好的动态库在使用时的模块名称
    "example module doc string", // 对应于“模块名.__doc__”, 也不是一个很重要的字段
    -1,
    MathMethods,
    NULL,
    NULL,
    NULL,
    NULL
};
 
static PyObject* __mathError;

// 这里的函数名称似乎非常重要, 必须采用“pyInit_模块名”的模式命名, 斗则无法从编译得到的动态库中import指定模块名称
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

### 2.