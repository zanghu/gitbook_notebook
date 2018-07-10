## Python: 用C/C++编写Python——官方文档

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

### 4.PyTypeObject的tp_name属性

参考资料：[const char* PyTypeObject.tp_name](https://docs.python.org/3.7/c-api/typeobj.html#c.PyTypeObject.tp_name)

Pointer to a NUL-terminated string containing the name of the type. For types that are accessible as module globals, the string should be the full module name, followed by a dot, followed by the type name; for built-in types, it should be just the type name. If the module is a submodule of a package, the full package name is part of the full module name. For example, a type named T defined in module M in subpackage Q in package P should have the tp_name initializer "P.Q.M.T".

`PyTypeObject.tp_name`指向一个表示类型的名字的C风格字符串。

对于那些可被作为模块全局变量访问的类型来说，这个字符串应该是：模块全名.类型名；

对于那些内建类型来说，字符串应该只包含类型名。

如果当前模块是一个程序包的某个子模块，那么程序包全名就是模块全名的一部分。举例来说：一个名为T的类型，定义在模块M中，该模块M包含在程序包Q中，而Q又是程序包P的子程序包，那么类型T的tp_namke属性应初始化为“P.Q.M.T”。

For dynamically allocated type objects, this should just be the type name, and the module name explicitly stored in the type dict as the value for key '__module__'.

For statically allocated type objects, the tp_name field should contain a dot. Everything before the last dot is made accessible as the __module__ attribute, and everything after the last dot is made accessible as the __name__ attribute.

If no dot is present, the entire tp_name field is made accessible as the __name__ attribute, and the __module__ attribute is undefined (unless explicitly set in the dictionary, as explained above). This means your type will be impossible to pickle. Additionally, it will not be listed in module documentations created with pydoc.

This field is not inherited by subtypes.

