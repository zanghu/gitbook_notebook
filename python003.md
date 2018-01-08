# \[Python\]numpy的array方法和asarray方法的区别

array和asarray都可以将结构数据转化为ndarray，但是主要区别就是当数据源是ndarray时，array仍然会copy出一个副本，占用新的内存，但asarray不会。

参考链接: [http://blog.csdn.net/gobsd/article/details/56485177(http://blog.csdn.net/gobsd/article/details/56485177)

