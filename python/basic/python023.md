## Python: 设置字典默认值

很久以前学习`pylearn2`源码的时候就见到过“为了防止查询的key不存在而设置默认值”这一用法，后面自己编程时也遇到过此类需求，但是已经不记得是该用哪个模块了。这次阅读`CTW`比赛的baseline代码再次见到，希望记下来备忘。

### 方法一：collections.defaultdict解决的问题

可以在创建defaultdict对象时指定元素类型，对于不同类型元素会自动赋一个默认初值。

Python中一般dict查找key-val的方法是`val = d[key]`，但是如果key不在val中，那么这个表达式就会抛出`KeyError`异常。

`collections.defaultdict`类型的字典对象与一般的dict功能无异，不过支持用户设置一个默认值，当根据key查询val时如果被查询的key不在当前字典中，那么会自动将该key插入字典并赋以之前设置的默认值。

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

### 方法二：使用dict类的`setdefault`在插入指定key时为防止该key不存在设置临时一个默认值

```python
>>>cn ={}
# 认为字典cn中每个val都是一个列表, 现在向cn['abc']列表中插入一个元素'123'，为防'abc'不存在于cn字典中，使用setdefault方法设置默认值

>>>cn.setdefault('abc',[]).append('123') 
>>>print cn
{'123':['123']}
```



