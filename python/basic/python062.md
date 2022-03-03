## Python: Python中方法（method）与函数（function）的区别

**参考资料**

segmentfault: [Python 面向对象-如何查看类的父类，外部如何获取类的名字](https://blog.csdn.net/qq_38161040/article/details/89501291)

### 方法

利用对象的`__base__`属性

```python
>>> np.ndarray.__base__
<class 'object'>
```
