## Python: 用C/C++扩展Python——官方文档目录

### 1.官方教程

参考资料：[官方教程目录](https://docs.python.org/3/extending/index.html)

参考资料: [1. Extending Python with C or C++](https://docs.python.org/3/extending/extending.html)

参考资料: [2. Defining Extension Types: Tutorial](https://docs.python.org/3/extending/newtypes_tutorial.html)

参考资料：[3. Defining Extension Types: Assorted Topics](https://docs.python.org/3/extending/newtypes.html)

参考资料：[4. Building C and C++ Extensions](https://docs.python.org/3/extending/building.html)

参考资料：[5. Building C and C++ Extensions on Windows](https://docs.python.org/3/extending/windows.html)

### 2.API

参考资料: [Python/C API Reference Manual](https://docs.python.org/3/c-api/index.html#c-api-index)

### 3.格式串

参考资料：[Parsing arguments and building values](https://docs.python.org/3.7/c-api/arg.html)

### 4.PyTypeObject的tp\_name属性

参考资料：[const char\* PyTypeObject.tp\_name](https://docs.python.org/3.7/c-api/typeobj.html#c.PyTypeObject.tp_name)

`PyTypeObject.tp_name`指向一个表示类型的名字的C风格字符串。`tp_name`赋值应遵循下面三条原则：

* 对于那些可被作为模块全局变量访问的类型来说，字符串`tp_name`应该是：模块全名.类型名；

* 对于那些内建类型来说，字符串`tp_name`应该只包含类型名。

* 如果当前模块是一个程序包的某个子模块，那么程序包全名就是模块全名的一部分。举例来说：一个名为T的类型，定义在模块M中，该模块M包含在程序包Q中，而Q又是程序包P的子程序包，那么类型T的`tp_name`属性应初始化为“P.Q.M.T”。

对于动态分配的类型对象，其`tp_name`属性值应该只包含类型名，而模块名则显示的作为键'**module**'的值保存在类型字典中。

  对于静态分配的类型对象，其`tp_name`属性值应该是一个由若干个由'.'分割的子串组成的字符串。`tp_name`最后一个'.'之前的部分就是类型对象的`__module__`属性的值（即可以通过类型对象的`__module__`属性访问），而最后一个'.'之后的部分则是类型对象的`__name__`的值（即可以通过类型对象的`__name__`属性访问）。

  如果类型对象的`tp_name`值中没有'.'，那么类型对象的`__name__`属性值就是`tp_name`的值，同时`__module__`属性处于未定义状态\*\(unless explicitly set in the dictionary, as explained above\)。这意味着该类型对象将无法被pickle，另外也不会出现在由pydoc创建的模块文档中。

  类型的`tp_name`属性不会被派生类型所继承。

