## Python: 用C/C++扩展Python——实践注意事项

本文内容主要针对实践中实际遇到的问题，在解决过程中查阅资料发现的知识点的笔记。文中的很多知识点事实上已经在Python官方参考资料中有提到。

#### 1.返回值: None和NULL

参考资料: (Method without return value in python c extension module)[https://stackoverflow.com/questions/8450481/method-without-return-value-in-python-c-extension-module]

All python functions should return a PyObject, unless when they want to raise an exception, as explained: here http://docs.python.org/extending/extending.html#intermezzo-errors-and-exceptions

The error message you get SystemError: error return without exception set, is trying to tell you that your function returned NULL (=error, raise an exception) but did not inform the python interpreter what exception you wanted to raise.

When you don't want to return a value from a python function you make it return None (which is same thing that happens if you in python code have a function that runs to the end or does a simple return without any value).

In the cpython api you do this by returning the Py_None object, and don't forget to increment its refcount. To help you not forgetting the refcount there is a macro to do it for you: http://docs.python.org/c-api/none.html#Py_RETURN_NONE.
