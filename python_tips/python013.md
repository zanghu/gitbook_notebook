## Python: 用C/C++扩展Python——实践注意事项

本文内容主要针对实践中实际遇到的问题，在解决过程中查阅资料发现的知识点的笔记。文中的很多知识点事实上已经在Python官方参考资料中有提到。

#### 1.返回值: None和NULL

参考资料: (Method without return value in python c extension module)[https://stackoverflow.com/questions/8450481/method-without-return-value-in-python-c-extension-module]

* **异常的一般处理范式**

When a function f that calls another function g detects that the latter fails, f should itself return an error value (usually NULL or -1). It should not call one of the PyErr_*() functions — one has already been called by g. f’s caller is then supposed to also return an error indication to its caller, again without calling PyErr_*(), and so on — the most detailed cause of the error was already reported by the function that first detected it. Once the error reaches the Python interpreter’s main loop, this aborts the currently executing Python code and tries to find an exception handler specified by the Python programmer.

(There are situations where a module can actually give a more detailed error message by calling another PyErr_*() function, and in such cases it is fine to do so. As a general rule, however, this is not necessary, and can cause information about the cause of the error to be lost: most operations can fail for a variety of reasons.)

C/C++编写的所有扩展Python函数都应该返回一个指向PyObject对象的指针，只有一种情况是例外，即当函数希望主动引发异常的时候。

一个重要的、从始至终贯彻于Python解释器设计的的原则是：当一个函数失败时，它应该设置异常条件并但会错误码（在C/C++编写的Python函数中，对应的行为一般是返回NULL指针）。

* **异常处理API**

Python提供了一系列API，包含了用来定义各种不同类型异常的函数。此类函数中最常用的那些包括：

`PyErr_SetString()`
`PyErr_SetFromErrno()`
`PyErr_SetObject()`

* **异常的三要素**

异常被保存在解释器内部的全局静态变量中；当没有异常发生时该变量是NULL.

此外还有一个全局变量存储异常的“伴随值”（即Python中raise函数的第二个参数）。

```python
raise [Exception [, args [, traceback]]] # 附注：Python中的raise函数的原型

```

Exceptions are stored in a static global variable inside the interpreter; if this variable is NULL no exception has occurred. A second global variable stores the “associated value” of the exception (the second argument to raise). A third variable contains the stack traceback in case the error originated in Python code. These three variables are the C equivalents of the result in Python of sys.exc_info() (see the section on module sys in the Python Library Reference). It is important to know about them to understand how errors are passed around.






The error message you get SystemError: error return without exception set, is trying to tell you that your function returned NULL (=error, raise an exception) but did not inform the python interpreter what exception you wanted to raise.

When you don't want to return a value from a python function you make it return None (which is same thing that happens if you in python code have a function that runs to the end or does a simple return without any value).

In the cpython api you do this by returning the Py_None object, and don't forget to increment its refcount. To help you not forgetting the refcount there is a macro to do it for you: http://docs.python.org/c-api/none.html#Py_RETURN_NONE.
