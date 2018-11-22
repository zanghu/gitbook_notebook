## Python: 逻辑运算符and和or的返回值

参考资料：[深入分析python的and or 返回值](https://www.jianshu.com/p/d4059a76a7a3)

### 0.问题引出

最近在参考CTW数据集基准方案代码`ctw-baseline`时，看到了以下源码：

```python
    ...
    # 注意下面一行代码中字典的'text'键的值
    a.append({'bbox': bbox, 'text': text or '■', 'color': colormap[taken]})
    ...
```

上面代码很明显是将表达式`text or '■'`赋值给字典的''text'键。

按照之前的理解，逻辑表达式都返回一个布尔值，但是这里'text'键的值显然应该是一个字符串（CTW样本中一个bbox的中文类标），这说明与之前对`or`和`and`的理解是错的。




