## Pandas：DataFrame的切片

参考资料：


### 1.按“列名”取若干列

取出`DataFrame`中列名为'A'和'B'的两列

```python
>>> df
   A   B   C
0  0   1   2
1  3   4   5
2  6   7   8
3  9  10  11
>>> df.columns
Index(['A', 'B', 'C'], dtype='object')
>>> df[['A', 'B']]
   A   B
0  0   1
1  3   4
2  6   7
3  9  10
```

> 注意:
> 如果只取1列，则有两种写法：`df[[列名]]`和`df[列名]`
> 前者返回的是一个`DataFrame`对象，而后者返回的是一个`Series`对象

### 2.按照列的“序号”取若干列

使用`DataFrame`的`iloc`方法。

取连续的列：取出`DataFrame`中列号为1和2的列（即'B'和'C'列）

```python
>>> df.iloc[:, 1:3]
    B   C
0   1   2
1   4   5
2   7   8
3  10  11
```

取不连续的列：取出`DataFrame`中列号为0和2的列（即A'和'C'列）

```python
>>> df.iloc[:, [0, 2]]
   A   C
0  0   2
1  3   5
2  6   8
3  9  11
```

### 3.列名和列序号之间相互转换

根据列名获取序号

```python
>>> df.columns.get_loc('A')
0
```

根据序号获取列名

```python
>>> df.columns[0]
'A'
```
