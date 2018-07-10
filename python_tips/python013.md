## Python: 用C/C++扩展Python——实践注意事项

参考资料：[教程目录](https://docs.python.org/3/extending/index.html)

参考资料: [1. Extending Python with C or C++](https://docs.python.org/3/extending/extending.html)

参考资料: [2. Defining Extension Types: Tutorial](https://docs.python.org/3/extending/newtypes_tutorial.html)

参考资料：[3. Defining Extension Types: Assorted Topics](https://docs.python.org/3/extending/newtypes.html)

参考资料：[4. Building C and C++ Extensions](https://docs.python.org/3/extending/building.html)

参考资料：[5. Building C and C++ Extensions on Windows](https://docs.python.org/3/extending/windows.html)

#### 1.返回值: None和NULL

参考资料: (Method without return value in python c extension module)[https://stackoverflow.com/questions/8450481/method-without-return-value-in-python-c-extension-module]

All python functions should return a PyObject, unless when they want to raise an exception, as explained: here http://docs.python.org/extending/extending.html#intermezzo-errors-and-exceptions

The error message you get SystemError: error return without exception set, is trying to tell you that your function returned NULL (=error, raise an exception) but did not inform the python interpreter what exception you wanted to raise.

When you don't want to return a value from a python function you make it return None (which is same thing that happens if you in python code have a function that runs to the end or does a simple return without any value).

In the cpython api you do this by returning the Py_None object, and don't forget to increment its refcount. To help you not forgetting the refcount there is a macro to do it for you: http://docs.python.org/c-api/none.html#Py_RETURN_NONE.
