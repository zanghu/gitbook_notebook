## Pandas：info和describe

参考资料：


### 1.查看数据基本信息：DataFrame.info()

```python
>>> df[['isDefault']].info()
<class 'pandas.core.frame.DataFrame'>
RangeIndex: 800000 entries, 0 to 799999
Data columns (total 1 columns):
isDefault    800000 non-null int64
dtypes: int64(1)
memory usage: 6.1 MB
```