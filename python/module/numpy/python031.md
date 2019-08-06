## numpy: 常用函数汇总

### numpy.array

`array`和`asarray`都可以将结构数据转化为`numpy.ndarray`类型对象，主要区别是

（1）当输入数据源是`numpy.ndarray`类型时，`array`返回的是拷贝（copy）的一个副本，`asarray`返回的是原对象的一个视图v（iew，类似引用)。

（2）当输入数据源不是`numpy.ndarray`类型时，二者返回的都是copy的副本。这时`array`和`asarray`没有区别，都对元数据进行了复制。

### numpy/asarray

见`numpy.array`

### numpy.ndarray.flatten

`ndarray.flatten`与`reval`功能是一致的，都是将多维数组降位一维。两者的区别：

（1）当输入数据源是`numpy.ndarray`类型时，`numpy.ndarray.flatten`返回拷贝，对拷贝所做的修改不会影响（reflects）原始矩阵，而`numpy.ravel`返回的是视图（view），会影响（reflects）原对象；

（2）当输入数据源不是`numpy.ndarray`类型时，`numpy.ravel`返回的也是copy的副本，而`numpy.ndarray.flatten`会报错。

### numpy.ravel

见`numpy.ndarray.flatten`

### numpy.take

根据索引号从一个`ndarray`中一次取出若干个元素组成一个新的`ndarray`



