## Python: Python中方法（method）与函数（function）的区别

**参考资料**

egmentfault: [Python: 函数与方法的区别 以及 Bound Method 和 Unbound Method](https://segmentfault.com/a/1190000009157792)

### 1.概念

随着我们越来越频繁使用Python, 我们难免会接触到类, 接触到类属性和方法.但是很多新手包括我, 不知道方法 和 函数 的区别,这次简单来讨论下, 如果有哪里认识不正确, 希望大神提点指教!
先来看两个定义吧:

>function（**函数**）—— A series of statements which returns some value toa caller. It can also be passed zero or more arguments which may beused in the execution of the body.
method（**方法**）—— A function which is defined inside a class body. If called as an attribute of an instance of that class, the method will get the instance object as its first argument (which isusually called self).

从上面可以看出, 别的编程语言一样, Function也是包含一个函数头和一个函数体, 也同样支持0到n个形参,而Method则是在function的基础上, 多了一层类的关系, 正因为这一层类, 所以区分了 function 和 method.而这个过程是通过 PyMethod_New实现的



