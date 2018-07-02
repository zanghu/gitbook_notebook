## Python: 为什么函数默认参数不应该是list类型

参考资料: [http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html](http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html)

#### 1.使用C/C++编写Python类

##### 1.1 基本步骤

* **第零部分**：定义C结构体，Python没有C++接口, 所以定义C语言的struct而不是C++的class

* **第一部分**：包装C/C++类方法

一般范式如下：

```cpp
static [void | PythonObject *] 方法名(结构体名称 * Self, PyObject *参数1, PyObject *参数2, ...)
{
    // 函数体
}
```

* **第二部分**：定义Python类成员

一般范式如下：

```cpp
static PyMemberDef 方法数组变量名称[] = 
{
    // {导出Python类属性名, 数据类型, offsetof(C结构体名称, C结构体属性名), 访问控制标识, 属性描述}
    {(char *)"m_szName",   T_STRING, offsetof(CScore, m_szName), READONLY, (char *)"The Name of instance"}, 

    /* 每个类属性占一行 *
    /
    {NULL}
};
```

* **第三部分**：定义Python类方法

一般范式如下：

```cpp
static PyMethodDef CScore_MethodMembers[] =      //类的所有成员函数结构列表.
{
    // {导出Python类属性名, C结构体方法名, 方法参数类型, 方法描述}
    {"GetName", (PyCFunction)CScore_GetName, METH_NOARGS, "Get the name of instance."},

    /* 每个类方法一行 */

    {NULL, NULL}
};
```

* **第四部分**：配置Python类定义

示例代码如下:

```cpp
static PyTypeObject CScore_ClassInfo =
{
 ...
};
```

* **第五部分**：将类加入模块

示例代码如下：

```cpp
PyMODINIT_FUNC PyInit_导出Python模块名(void)
{
    ...
    pReturn = PyModule_Create(&ModuleInfo); // 根据模块信息创建模块.
    ...

    // 类加入模块
    Py_INCREF(&ModuleInfo);
    PyModule_AddObject(pReturn, "CScore", (PyObject*)&CScore_ClassInfo); 

    return pReturn;
}
```