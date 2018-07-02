## Python: 为什么函数默认参数不应该是list类型

参考资料: [http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html](http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html)

#### 1.使用C/C++编写Python类

##### 1.1 基本步骤

* 第零部分：定义C结构体，Python没有C++接口, 所以定义C语言的struct而不是C++的class

* 第一部分：包装C/C++类方法

一般范式：

```cpp
static [void | PythonObject *] 方法名(结构体名称 * Self, PyObject *参数1, PyObject *参数2, ...)
{
    // 函数体
}
```


* 第二部分：定义Python类成员

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


* 第三部分：定义Python类方法
static PyMethodDef CScore_MethodMembers[] =      //类的所有成员函数结构列表.
{
    // {导出Python类属性名, C结构体方法名, 方法参数类型, 方法描述}
    {"GetName", (PyCFunction)CScore_GetName, METH_NOARGS, "Get the name of instance."},
   
    /* 每个类方法一行 */
   
    {NULL, NULL}
};


* 第四部分：配置Python类定义

////////////////////////////////////////////////////////////
// 类/结构的所有成员、内置属性的说明信息.
//
static PyTypeObject CScore_ClassInfo =
{
    PyVarObject_HEAD_INIT(NULL, 0)"Module.MyCppClass",                 //可以通过__class__获得这个字符串. CPP可以用类.__name__获取.
    sizeof(CScore),                 //类/结构的长度.调用PyObject_New时需要知道其大小.
    0,
    (destructor)CScore_Destruct,    //类的析构函数.
    0,
    0,
    0,
    0,
    (reprfunc)CScore_Repr,          //repr 内置函数调用。
    0,
    0,
    0,
    0,
    0,
    (reprfunc)CScore_Str,          //Str/print内置函数调用.
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                 //如果没有提供方法的话，为Py_TPFLAGS_DEFAULE
    "MyCppClass Objects---Extensioned by C++!",                   //__doc__,类/结构的DocString.
    0,
    0,
    0,
    0,
    0,
    0,
    CScore_MethodMembers,        //类的所有方法集合.
    CScore_DataMembers,          //类的所有数据成员集合.
    0,
    0,
    0,
    0,
    0,
    0,
    (initproc)CScore_init,      //类的构造函数.
    0,
};


/* 附加部分：定义Python模块 */
////////////////////////////////////////////////////////////
// 此模块的说明信息.
//
static PyModuleDef ModuleInfo =
{
    PyModuleDef_HEAD_INIT,
    "My C++ Class Module",               //模块的内置名--__name__.
    "This Module Created By C++--extension a class to Python!",                 //模块的DocString.__doc__
    -1,
    NULL, NULL, NULL, NULL, NULL
};


/* 第五部分：导出Python模块 + 将类加入模块 */
