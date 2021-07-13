## Pandas：统计列取值的种类

### 参考资料：

知乎: [Pandas中的object数据类型｜Python](https://zhuanlan.zhihu.com/p/161073173)

pbpython: [Overview of Pandas Data Types](https://pbpython.com/pandas_dtypes.html)

### 0.样例数据

数据如下，其中'A'和'B'取值为0或1，'C'列取值在['A', 'B', 'C']中

```python
>>> df
   A  B  C
0  1  0  A
1  0  1  A
2  1  1  B
3  1  1  C
```

### 1.统计单列数据

方法：利用`value_counts`

```python
>>> df['A'].value_counts()
1    3
0    1
Name: A, dtype: int64
```

利用`DataFrame`对象的“列名即属性”的特性，也可以按如下方式写：

```python
>>> df.A.value_counts()
1    3
0    1
Name: A, dtype: int64
```

### 2.统计全部列数据

方法：利用`apply`和`value_counts`

```python
>>> df.apply(pd.value_counts)
     A    B    C
0  1.0  1.0  NaN
1  3.0  3.0  NaN
A  NaN  NaN  2.0
B  NaN  NaN  1.0
C  NaN  NaN  1.0
```

> 注意：如果各列的取值范围差别较大，不宜整体统计

