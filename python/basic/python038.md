## 为什么Python没有函数重载

参考资料：

重载这个特性在很多语言中都有实现，比如 C++、Java 等，而 Python 并不支持。这篇文章呢，通过一些小技巧，可以让 Python 支持类似的功能。

### 1.需要函数重载的场景

* 函数参数个数不同

* 函数参数类型不同

### 2.参数个数不同的情形

先看看这种情况下 C++ 是怎么实现重载的

```cpp
#include <iostream>
using namespace std;
 
int func(int a)
{
    cout << 'One parameter' << endl;
}
 
int func(int a, int b)
{
    cout << 'Two parameters' << endl;
}
 
int func(int a, int b, int c)
{
    cout << 'Three parameters' << endl;
}
```

如果 Python 按类似的方式定义函数的话，不会报错，只是后面的函数定义会覆盖前面的，达不到重载的效果。

```python
>>> def func(a):
...   print('One parameter')
... 
>>> def func(a, b):
...   print('Two parameters')
... 
>>> def func(a, b, c):
...   print('Three parameters')
... 
>>> func(1)
Traceback (most recent call last):
 File "<stdin>", line 1, in <module>
TypeError: func() missing 2 required positional arguments: 'b' and 'c'
>>> func(1, 2)
Traceback (most recent call last):
 File "<stdin>", line 1, in <module>
TypeError: func() missing 1 required positional argument: 'c'
>>> func(1, 2, 3)
Three parameters
```

但是我们知道，Python 函数的形参十分灵活，我们可以只定义一个函数来实现相同的功能，就像这样

```python
>>> def func(*args):
...   if len(args) == 1:
...     print('One parameter')
...   elif len(args) == 2:
...     print('Two parameters')
...   elif len(args) == 3:
...     print('Three parameters')
...   else:
...     print('Error')
... 
>>> func(1)
One parameter
>>> func(1, 2)
Two parameters
>>> func(1, 2, 3)
Three parameters
>>> func(1, 2, 3, 4)
Error
```

### 参3.数类型不同的情形

同样，先看下当前情况下 C++ 的重载是怎么实现的

```cpp
#include <iostream>
using namespace std;
 
int func(int a)
{
    cout << 'Int: ' << a << endl;
}
 
int func(float a)
{
    cout << 'Float: ' << a << endl;
}
```

代码中，func 支持两种类型的参数：整形和浮点型。调用时，解释器会根据参数类型去寻找合适的函数。Python 要实现类似的功能，需要借助 functools.singledispatch 装饰器。

```python
from functools import singledispatch
 
@singledispatch
def func(a):
    print(f'Other: {a}')
 
@func.register(int)
def _(a):
    print(f'Int: {a}')
 
@func.register(float)
def _(a):
    print(f'Float: {a}')
 
if __name__ == '__main__':
    func('zzz')
    func(1)
    func(1.2)
```

func 函数被 functools.singledispatch 装饰后，又根据不同的参数类型绑定了另外两个函数。当参数类型为整形或者浮点型时，调用绑定的对应的某个函数，否则，调用自身。

执行结果

Other: zzz
Int: 1
Float: 1.2

需要注意的是，这种方式只能够根据第一个参数的类型去确定最后调用的函数。