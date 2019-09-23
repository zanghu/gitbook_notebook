## Python源码阅读（一）: Python的面向对象机制

参考资料：[深入源码解析Python中的对象与类型](https://www.php.cn/python-tutorials-157807.html)


### 对象

Python中对象分为两类: 定长(int等), 非定长(list/dict等容器)

所有对象都有一些相同的东西, 源码中定义为PyObject和PyVarObject, 两个定义都有一个共同的头部定义PyObject_HEAD(其实PyVarObject有自己的头部定义PyObject_VAR_HEAD, 但其实际上用的也是PyObject_HEAD).

源码位置: Include/object.h

### PyObject_HEAD & PyObject_VAR_HEAD

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


### PyObject & yVarObject

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





### 基类的操作宏函数

* 属性相关方法
```c
#define Py_REFCNT(ob)           (((PyObject*)(ob))->ob_refcnt)  // 读取引用计数
#define Py_TYPE(ob)             (((PyObject*)(ob))->ob_type)    // 获取对象类型
#define Py_SIZE(ob)             (((PyVarObject*)(ob))->ob_size) //读取元素个数(len)
```

* 跟对象相关的方法
```c
#define Py_REFCNT(ob) ... // 增加对象引用计数
#define Py_DECREF(ob) ... // 减少对象引用计数, 如果计数位0, 调用_Py_Dealloc
```



我们反向推导一个int对象是怎么生成的.

1. 首先, 定义一种类型叫PyTypeObject
代码位置 Include/object.h

定义

























typedef struct _typeobject {

 

 /* MARK: base, 注意, 是个变长对象*/

 PyObject_VAR_HEAD

 const char *tp_name; /* For printing, in format "<module>.<name>" */ //类型名

 Py_ssize_t tp_basicsize, tp_itemsize; /* For allocation */ // 创建该类型对象时分配的内存空间大小

 

 

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

 

</name></module>

说明

1. PyObject_VAR_HEAD
变长对象

2. const char *tp_name
tp_name, 类型名字符串数组
所有Type都是PyTypeObject的"实例": PyType_Type/PyInt_Type

2. 然后, 用PyTypeObject初始化得到一个对象PyType_Type
代码位置 Objects/typeobject.c

定义












PyTypeObject PyType_Type = {

 PyVarObject_HEAD_INIT(&PyType_Type, 0)

 "type",                   /* tp_name */

 sizeof(PyHeapTypeObject),          /* tp_basicsize */

 sizeof(PyMemberDef),            /* tp_itemsize */

 (destructor)type_dealloc,          /* tp_dealloc */

 

 // type对象的方法和属性初始化值

 .....

 

};

说明

1. tp_name
类型名, 这里是"type"

2. PyVarObject_HEAD_INIT(&PyType_Type, 0)
PyVarObject_HEAD_INIT, 这个方法在 Include/object.h中,
等价于
ob_refcnt = 1
*ob_type = &PyType_Type
ob_size = 0

即, PyType_Type的类型是其本身!
结构

第一张图, 箭头表示实例化(google doc用不是很熟找不到对应类型的箭头)
20151211181832844.png (510×277)

第二张图, 箭头表示指向
20151211181846535.png (484×250)

使用










# 1. int 的 类型 是`type`

>>> type(int)

<type 'type'="">

 

# 2. type 的类型 还是`type`, 对应上面说明第二点

>>> type(type(int))

<type 'type'="">

 

</type></type>

注意: 无论任何时候, ob_type指向的是 PyTypeObject的实例: PyType_Type/PyInt_Type...

3. 再然后, 定义具体的类型, 这里以PyInt_Type为例子
代码位置 Objects/intobject.c

定义













PyTypeObject PyInt_Type = {

 PyVarObject_HEAD_INIT(&PyType_Type, 0)

 "int",

 sizeof(PyIntObject),

 0,

 

 // int类型的相关方法和属性值

 ....

 

 (hashfunc)int_hash,             /* tp_hash */

 

};

说明

1. "int"
PyInt_Type的类型名是int

2.PyVarObject_HEAD_INIT(&PyType_Type, 0)
PyInt_Type的


*ob_type = &PyType_Type

结构
20151211181940069.png (1083×570)

使用








>>> type(1)

<type 'int'="">

 

>>> type(type(1))

<type 'type'="">

 

</type></type>

4. 最后, 生成一个整数对象int
代码位置 Include/intobject.h

定义





typedef struct {

  PyObject_HEAD

  long ob_ival;

} PyIntObject;

结构
20151211182004246.png (1768×832)

微信
分享