## Python: 带默认值的字典collections.defaultdict

很久以前学习`pylearn2`源码的时候就见到过`collections.defaultdict`的使用，后面自己编程时也遇到过此类需求，但是已经不记得是该用哪个模块了。这次阅读`CTW`比赛的baseline代码再次见到，希望记下来备忘。

### 1.collections.defaultdict解决的问题

Python中一般dict查找key-val的方法是`val = d[key]`，但是如果key不在val中，那么这个表达式就会抛出`KeyError`异常。

`collections.defaultdict`类型的字典对象与一般的dict功能无异，不过支持用户设置一个默认值，当根据key查询val时如果被查询的key不在当前字典中，那么会自动将该key插入字典并赋以之前设置的默认值。

### 2.defaultdict默认值的设置方法

* **方法一**：可以在创建defaultdict对象时指定元素类型，对于不同类型元素会自动赋一个默认初值。

```python
>>>from collections import defaultdict
>>>a = defaultdict(list) # list型元素默认值是空列表
>>>a['x']
[]
>>>b = defaultdict(int)# int型元素默认值是0
>>>b['x']
0
>>>c = defaultdict(str)# str型元素默认值是空字符串
>>>c['x']
''
```
