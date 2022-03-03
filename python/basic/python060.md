## Python: 如何区分类或对象的成员究竟是方法还是成员

**参考资料**

cnblogs：[python str与bytes之间的转换](https://www.cnblogs.com/zqifa/p/python-7.html)

### 1.显示对象或类的全部成员

Python具有强大的反射功能，可以用`__class__`告诉调用者“我是谁”、也可以用`dir()`通过显示对象或类的全部成员来说明“我会什么”。

**示例**：


```python
>>> import numpy as np
>>> x = np.arange(12).reshape(4, 3)
>>> x.__class__
<class 'numpy.ndarray'>
>>> dir(x)
['T', '__abs__', '__add__', '__and__', '__array__', '__array_finalize__', '__array_function__', '__array_interface__', '__array_prepare__', '__array_priority__', '__array_struct__', '__array_ufunc__', '__array_wrap__', '__bool__', '__class__', '__complex__', '__contains__', '__copy__', '__deepcopy__', '__delattr__', '__delitem__', '__dir__', '__divmod__', '__doc__', '__eq__', '__float__', '__floordiv__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__iadd__', '__iand__', '__ifloordiv__', '__ilshift__', '__imatmul__', '__imod__', '__imul__', '__index__', '__init__', '__init_subclass__', '__int__', '__invert__', '__ior__', '__ipow__', '__irshift__', '__isub__', '__iter__', '__itruediv__', '__ixor__', '__le__', '__len__', '__lshift__', '__lt__', '__matmul__', '__mod__', '__mul__', '__ne__', '__neg__', '__new__', '__or__', '__pos__', '__pow__', '__radd__', '__rand__', '__rdivmod__', '__reduce__', '__reduce_ex__', '__repr__', '__rfloordiv__', '__rlshift__', '__rmatmul__', '__rmod__', '__rmul__', '__ror__', '__rpow__', '__rrshift__', '__rshift__', '__rsub__', '__rtruediv__', '__rxor__', '__setattr__', '__setitem__', '__setstate__', '__sizeof__', '__str__', '__sub__', '__subclasshook__', '__truediv__', '__xor__', 'all', 'any', 'argmax', 'argmin', 'argpartition', 'argsort', 'astype', 'base', 'byteswap', 'choose', 'clip', 'compress', 'conj', 'conjugate', 'copy', 'ctypes', 'cumprod', 'cumsum', 'data', 'diagonal', 'dot', 'dtype', 'dump', 'dumps', 'fill', 'flags', 'flat', 'flatten', 'getfield', 'imag', 'item', 'itemset', 'itemsize', 'max', 'mean', 'min', 'nbytes', 'ndim', 'newbyteorder', 'nonzero', 'partition', 'prod', 'ptp', 'put', 'ravel', 'real', 'repeat', 'reshape', 'resize', 'round', 'searchsorted', 'setfield', 'setflags', 'shape', 'size', 'sort', 'squeeze', 'std', 'strides', 'sum', 'swapaxes', 'take', 'tobytes', 'tofile', 'tolist', 'tostring', 'trace', 'transpose', 'var', 'view']
```

### 2.判断成员是方法还是属性

#### 2.1 检查__call__

如果某个对象具备`__call__`成员，则它可以被一函数的形式调用，也就可以被认为是一个方法，反之就是一个成员。

**示例**：

```python
>>> import numpy as np
>>> x = np.arange(12).reshape(4, 3)
>>> hasattr(x.shape, '__call__') # shape没有__call__成员，因此shape是属性
False
>>> hasattr(x.mean, '__call__') # mean没有__call__成员，因此mean是属性

True
```


