# \[Python\]关于staticmethod和calssmethod的区别

array和asarray都可以将结构数据转化为ndarray，但是主要区别是：

当数据源是numpy.ndarray时，array仍然会copy出一个副本，占用新的内存，但asarray不会。

当输入数据源不是numpy.ndarray类型时，二者返回的都是copy的副本。可见array和asarray没有区别，都对元数据进行了复制。

参考链接: \[[http://blog.csdn.net/gobsd/article/details/56485177\(http://blog.csdn.net/gobsd/article/details/56485177](http://blog.csdn.net/gobsd/article/details/56485177%28http://blog.csdn.net/gobsd/article/details/56485177)\)

