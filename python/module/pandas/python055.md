## Pandas：统计列取值的种类

### 参考资料：

CSDN: [Pandas中的object数据类型｜Python](https://zhuanlan.zhihu.com/p/161073173)

pbpython: [Overview of Pandas Data Types](https://pbpython.com/pandas_dtypes.html)

### 1.修改DataFarme对象的列顺序

```python
# 将重新排好序的列标题做成列表
order = ['date', 'time', 'open', 'high', 'low', 'close', 'volumefrom', 'volumeto']

# 切片列置换
df = df[order]
```
