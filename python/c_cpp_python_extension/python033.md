## Python源码阅读（一）: Python的类型体系

参考资料：[深入源码解析Python中的对象与类型](https://www.php.cn/python-tutorials-157807.html)

参考资料：[Python 的 type 和 object 之间是怎么一种关系？](https://www.zhihu.com/question/38791962)

参考资料：《Python源码剖析》

### 1.Python对象在C源码中的基类

#### 1.1 Python对象

Python有一句话：一切皆对象。因为所有Python运行时环境中的概念实际上都是一个C语言结构体的实例。

Python中类型间的一切关系本质上都是C语言实例的运行时对象间的关联关系。例如：Python中自定义类型A继承了`object`类型，其实是A类型的Python定义（也是Python运行时对象）关联着`object`类型（也是Python运行时对象），这种关联可能是通过子类对象中的一个指向基类的指针实现的。

作为对比，C语言中的类间继承关系是一种纯粹的语法特性上的关联关系，是通过定义的源码静态描述的。

Python中对象分为两类: 定长(int等), 非定长(list/dict等容器)

### 1.2 PyObject_HEAD & PyObject_VAR_HEAD

所有对象都有一些相同的东西, 源码中定义为PyObject和PyVarObject, 两个定义都有一个共同的头部定义PyObject_HEAD(其实PyVarObject有自己的头部定义PyObject_VAR_HEAD, 但其实际上用的也是PyObject_HEAD).

* 源码位置: `include/python3.6m/object.h`

Python 内部, 每个对象拥有相同的头部.

* 定义
```c
  /* PyObject_HEAD defines the initial segment of every PyObject. */
  // 所有定长对象的公共头部部分
  #define PyObject_HEAD          \
      _PyObject_HEAD_EXTRA \       // 先忽略, 双向链表结构, 有资料显示该宏只与DEBUG模式有关
      Py_ssize_t ob_refcnt; \      // 引用计数, 跟Python的内存管理机制相关
      struct _typeobject *ob_type; // 指向类型对象的指针(指向_typeobject结构体)
  
  /* PyObject_VAR_HEAD defines the initial segment of all variable-size container objects. */
  // 所有非定长对象的公共头部部分
  #define PyObject_VAR_HEAD        \
      PyObject_HEAD            \
      Py_ssize_t ob_size; /* Number of items in variable part */
```

* 说明
`PyObject_HEAD`可以看作是Python源码中所有Python对象的C定义中的基类，同时也是`PyObject_VAR_HEAD`的基类。
`PyObject_VAR_HEAD`则是Python中所有非定长对象的基类。


### 1.3 PyObject & PyVarObject

* 定义

```c
typedef struct _object {
  PyObject_HEAD
} PyObject;

typedef struct {
  PyObject_VAR_HEAD
} PyVarObject;
```

![](/assets/python033_01.png)
![](/assets/python033_02.png)


* 基类的操作宏函数

（1） 属性相关方法
```c
#define Py_REFCNT(ob)           (((PyObject*)(ob))->ob_refcnt)  // 读取引用计数
#define Py_TYPE(ob)             (((PyObject*)(ob))->ob_type)    // 获取对象类型
#define Py_SIZE(ob)             (((PyVarObject*)(ob))->ob_size) //读取元素个数(len)
```

（2） 跟对象相关的方法
```c
#define Py_REFCNT(ob) ... // 增加对象引用计数
#define Py_DECREF(ob) ... // 减少对象引用计数, 如果计数位0, 调用_Py_Dealloc
```

### 2.类型机制

Python中经常都提到“一切皆对象”的，甚至包括Python中的类型(class)的定义也是一个对象，所有类型的基类type也是一个对象。

我们反向推导一个int对象是怎么生成的，通过这个过程来说明Python源码中是如何通过C语言来实现“一切皆对象的”。

#### 2.1 首先, 定义一种类型叫PyTypeObject，Python中一切类型都是该定义的实例

代码位置：`include/python3.6m/object.h`

* 定义

```c
typedef struct _typeobject {
 /* MARK: base, 注意, 是个变长对象*/
 PyObject_VAR_HEAD // 变长对象基类
 const char *tp_name;  // 类型名，是一个字符串
 Py_ssize_t tp_basicsize, tp_itemsize; // 创建该类型对象时分配的内存空间大小

 // 一堆方法定义, 函数和指针
 /* Methods to implement standard operations */
 printfunc tp_print;
 hashfunc tp_hash;

 /* Method suites for standard classes */
 PyNumberMethods *tp_as_number;  // 数值对象操作
 PySequenceMethods *tp_as_sequence; // 序列对象操作
 PyMappingMethods *tp_as_mapping; // 字典对象操作

 // 一堆属性定义
 ....
} PyTypeObject;
```

* 说明

Python中的所有Type都是`PyTypeObject`的"实例"，包括所有类型的基类type自身也是`PyTypeObject`的实例。后面我们将看到，Python中的一切基本类型都是`PyTypeObject`的·实例。

#### 2.2 其次，定义一个PyTypeObject实例叫做PyType_Type，作为Python中所有类型的类型

PyType_Type是`PyTypeObject`的一个实例，它就是Python中一切类型的基类Type

```c
PyTypeObject PyType_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "type",                   /* tp_name */
    sizeof(PyHeapTypeObject),          /* tp_basicsize */
    sizeof(PyMemberDef),            /* tp_itemsize */
    (destructor)type_dealloc,          /* tp_dealloc */

    // type对象的方法和属性初始化值
    .....
};
```

* 说明

（1）`PyType_Type`是一个C语言全局变量；

（2）宏函数`PyVarObject_HEAD_INIT`的定义
```c
#define PyObject_HEAD_INIT(type)        \
    { _PyObject_EXTRA_INIT              \
    1, type },

#define PyVarObject_HEAD_INIT(type, size)       \
    { PyObject_HEAD_INIT(type) size },
```

（3）注意到：类型名`tp_name`这里是"type", 归属类型`*ob_type = &PyType_Type`，即, PyType_Type的类型是其本身!

![](/assets/python033_03.png) 
![](/assets/python033_04.png)

#### 2.3 其次，定义一个PyTypeObject实例叫做PyInt_Type，它在Python运行时的类型是type类型

* 代码位置`Objects/intobject.c`
  >注意：python3.6中没有`intobject.c`文件和`PyInt_Type`类型，代替它们的是`longobject.c`文件和`PyLong_Type`类型`。

* 定义

```c
PyTypeObject PyInt_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "int",
    sizeof(PyIntObject),
    0,

    // int类型的相关方法和属性值
    ....
    (hashfunc)int_hash,             /* tp_hash */
};
```

* 说明

（1）"int"：PyInt_Type的类型名是int

（2）*ob_type = &PyType_Type说明`PyInt_Type`作为Python的类型其Python意义下的归属类型是`PyType_Type`

![](/assets/python033_05.png)

#### 2.4 最后, 定义int类型的对象的C定义PyIntObject，它是PyObject的C语言意义下的子类

* 代码位置：`include/.../intobject.h`

* 定义

```c
typedef struct {
    PyObject_HEAD
    long ob_ival;
} PyIntObject;
```

![](/assets/python033_06.png)

