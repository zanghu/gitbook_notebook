## Pandas：isna() vs isnull()，二者有什么区别

### Pandas.DataFrame isna() vs isnull()
我觉得咱要说的是pandas.DataFrame.isna() vs pandas.DataFrame.isnull()。而不是pandas.isnull()，因为它不是用于`DataFrame`的。
这俩函数的功用一模一样！它俩甚至连官方文档都一模一样。你点击pandas的官网文档就明白了。

### 但是为什么搞2个函数、取2个名字，来做同样的事儿？
这是因为pandas的DataFrame是基于R的DataFrame。在R里边na和null是分开的2个东西。更多相关信息请参考这篇文章。

然而，在python里边，pandas是建在numpy上的（即numpy的定制版），而numpy可是没na，也没null，只有NaN的（是 “Not a Number”的缩写）。因此，pandas还用NaN。

### 简单说呢就是：
numpy里边查找NaN值的话，就用np.isnan()。
pandas里边查找NaN值的话，要么.isna()，要么.isnull()。
NaN源于这样一个事实：即pandas构建在numpy之上，而这两个函数的名称源自R的DataFrame，pandas就是试图模仿它的结构和功能。
