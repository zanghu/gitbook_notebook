## numpy: 常用函数汇总

### numpy.array

`array`和`asarray`都可以将结构数据转化为`numpy.ndarray`类型对象，主要区别是

（1）当原对象是`numpy.ndarray`类型时，`array`返回的是copy出的一个副本，`asarray`返回的是原对象的一个view(引用)。

（2）当输入数据源不是numpy.ndarray类型时，二者返回的都是copy的副本。可见array和asarray没有区别，都对元数据进行了复制。

### numpy/asarray

见`numpy.array`

### numpy.flatten

