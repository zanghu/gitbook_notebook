## Python: 操作excel表格

参考资料: [Method without return value in python c extension module](https://stackoverflow.com/questions/8450481/method-without-return-value-in-python-c-extension-module)

#### 1.正确设计C/C++扩展函数的返回值: None和NULL

参考资料: [Method without return value in python c extension module](https://stackoverflow.com/questions/8450481/method-without-return-value-in-python-c-extension-module)

参考资料: [Intermezzo: Errors and Exceptions](https://docs.python.org/3/extending/extending.html#intermezzo-errors-and-exceptions)

* C**函数返回值**

C/C++编写的所有扩展Python函数都应该返回一个指向PyObject对象的指针，只有一种情况是例外，即当函数希望主动引发异常的时候。

一个重要的、从始至终贯彻于Python解释器设计的的原则是：当一个函数失败时，它应该设置异常条件并但会错误码（在C/C++编写的Python函数中，对应的行为一般是返回NULL指针）。

如果一个函数本身并没有发生任何错误，也没有收到任何它所调用的函数返回的错误信息，那么如果该函数返回NULL，Python解释器就会发生异常，并提示下面的异常信息：`SystemError: error return without exception set`。这种问题往往出现在函数设计者想要返回`None`但却错误的反回了`NULL`的情况下，正确的做法是返回一个Python的None对象，例如：

```python
static PyObject *myfunction(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, "i", ...))
        return NULL;
    /* .... */
    Py_RETURN_NONE;
}
```

* **异常的一般处理范式**（非常重要）

假设有一个函数f内部调用了另一个函数g，调用后f发现g调用失败（比如：通过检查到g的返回值是某个约定的错误码），那么正确的做法是：

\(1\) f也应该返回一个标记错误发生的返回值（通常是NULL或-1）。一般来说，此类情况下f不应主动调用`PyErr_*()`函数——因为此函数应该已经被g在执行过程中调用过一次。

\(2\) 对于f的调用者来说，正确的做法是接下来也将一个错误标识值返回给自身的调用者，同样的也不要调用`PyErr_*()`.

\(3\) 以此类推（整个函数调用链上的函数都应该按照f的做法将错误信息逐级返回给自身调用者）。

这样一来，顶层调用者（一般是Python解释器）收到的异常信息就是最底层那个首先发生异常的函数所报告的最细致（这里的“detail”应该是指粒度最小、最底层和最基础）的异常信息。一旦错误到达Python解释器的主循环，就会立即中断当前正在执行的Python代码同时开始尝试找到程序中指定的该类型异常的处理句柄。

* **异常处理API**

Python提供了一系列API，包含了用来定义各种不同类型异常的函数。此类函数中最常用的那些包括：

`PyErr_SetString()`  
`PyErr_SetFromErrno()`  
`PyErr_SetObject()`

* **异常的三要素**

\(1\) 异常被保存在解释器内部的一个全局静态变量中；当没有异常发生时该变量是NULL.

\(2\) 此外还有一个全局变量存储异常的“伴随值”（即Python中raise函数的第二个参数）。

```python
raise [Exception [, args [, traceback]]] # 附注：Python中的raise函数的原型
```

语句中 Exception 是异常的类型（例如，NameError）参数标准异常中任一种，args 是自已提供的异常参数。最后一个参数是可选的（在实践中很少使用），如果存在，是跟踪异常对象。

\(3\) 此外还有第三个变量，用来在错误由Python代码引起的情况下保存堆栈信息。

这三个变量是Python函数`sys.exc_info()`返回值的C语言下的等价物。

### 2.继承C/C++扩展类需要注意的一件事

参考资料: [官方文档中的note](https://docs.python.org/3/extending/newtypes_tutorial.html)

使用C/C++扩展Python类型时，如果扩展子类与其Phython基类具有相同的`tp_basicsize`属性值，那么可能会在多重继承时出现问题。

避免此问题的方法是：确保自定义类型的tp_basicsize值大于其基类的值。大多数情况下，无论如何都是如此，因为您的基类型将是对象，否则您将向基类型添加数据成员，从而增加其大小。
