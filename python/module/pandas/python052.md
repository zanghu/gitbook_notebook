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