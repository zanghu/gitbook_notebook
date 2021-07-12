## Pandas：info和describe

参考资料：


### 1.查看数据基本信息：DataFrame.info()

显示`DataFrame`的行数、数据类型、内存占用大小

```python
>>> df[['isDefault']].info()
<class 'pandas.core.frame.DataFrame'>
RangeIndex: 800000 entries, 0 to 799999
Data columns (total 1 columns):
isDefault    800000 non-null int64
dtypes: int64(1)
memory usage: 6.1 MB
```

>注意：`info()`是`DataFrame`对象的方法，不是`Series`对象的方法。

### 2.查看数据统计信息：DataFrame.describe()

显示`DataFrame`的均值、方差、分位数等信息

```python
>>> df[['isDefault']].describe()
           isDefault
count  800000.000000
mean        0.199513
std         0.399634
min         0.000000
25%         0.000000
50%         0.000000
75%         0.000000
max         1.000000
```