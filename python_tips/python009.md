## Python: 为什么函数默认参数不应该是list类型

参考资料: [http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html](http://satran.in/2012/01/12/python-dangerous-default-value-as-argument.html)

#### 1.例子

```python
>>> def foo(x=[]):
...     x.append(1)
...     print x
... 
>>> foo()
[1]
>>> foo()
[1, 1]
>>> foo()
[1, 1, 1]
```

#### 2.问题原因

python解释器在解析函数定义时，会为每个默认参数创建一个对象，并进行一次绑定。之后再程序调用时，解释器不会在每次调用时都重新创建一次默认参数对象。

这就导致当函数默认参数不是不可变类型时，可能因为被修改而导致产生错误的结果。


### 3.解决办法

对于上面示例代码的问题，建议改为：

```python

```
