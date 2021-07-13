## Pandas：统计列取值的种类

参考资料：

StackOverflow: [Count the frequency that a value occurs in a dataframe column](https://stackoverflow.com/questions/22391433/count-the-frequency-that-a-value-occurs-in-a-dataframe-column)

### 0.样例数据

```python
>>> df
   A  B  C
0  1  0  A
1  0  1  A
2  1  1  B
3  1  1  C
```

### 1.单列数据统计

方法：利用`value_counts`

```python
>>> df['A'].value_counts()
1    3
0    1
Name: A, dtype: int64
```

