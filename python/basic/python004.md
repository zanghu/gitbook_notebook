# \[Python\]关于staticmethod和calssmethod的区别

### 经典回答1

@classmethod means: when this method is called, we pass the class as the first argument instead of the instance of that class \(as we normally do with methods\). This means you can use the class and its properties inside that method rather than a particular instance.

@staticmethod means: when this method is called, we don't pass an instance of the class to it \(as we normally do with methods\). This means you can put a function inside a class but you can't access the instance of that class \(this is useful when your method does not use the instance\).

简单的示例：

```python
class A(object):
    def m1(self, n):
        """普通方法"""
        print("self:", self)

    @classmethod
    def m2(cls, n):
        print("cls:", cls)

    @staticmethod
    def m3(n):
        pass

a = A()
a.m1(1) # self: <__main__.A object at 0x000001E596E41A90>
A.m2(1) # cls: <class '__main__.A'>
A.m3(1)
```

### 经典回答2

类的普通方法的调用方式

![](/assets/1.jpg)

类的类方法和静态方法的调用方式

![](/assets/2.jpg)

参考资料：https://www.zhihu.com/question/20021164

