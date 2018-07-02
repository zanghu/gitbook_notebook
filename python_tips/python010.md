## Python: 为什么函数默认参数不应该是list类型

参考资料: [http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html](http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html)

### 1.扩展部分的C++源码

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