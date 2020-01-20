# \[Python\]关于staticmethod和calssmethod的区别

**参考资料**

知乎：[正确理解Python中的 @staticmethod@classmethod方法](https://zhuanlan.zhihu.com/p/28010894)

### 经典回答1

@classmethod means: when this method is called, we pass the class as the first argument instead of the instance of that class \(as we normally do with methods\). This means you can use the class and its properties inside that method rather than a particular instance.

@staticmethod means: when this method is called, we don't pass an instance of the class to it \(as we normally do with methods\). This means you can put a function inside a class but you can't access the instance of that class \(this is useful when your method does not use the instance\).

简单的示例：

```python
class A(object):
    def m1(self, n):
        """普通方法，第一个参数self是类实例对象"""
        print("self:", self)

    @classmethod
    def m2(cls, n):
        """类方法, 第一个参数cls是类定义对象而不是类的实例对象"""
        print("cls:", cls)

    @staticmethod
    def m3(n):
        """静态方法，既不需要类定义对象也不需要类实例对象"""
        pass

a = A()
a.m1(1) # self: <__main__.A object at 0x000001E596E41A90>
A.m2(1) # cls: <class '__main__.A'>
A.m3(1)
```

