## Pandas：修改DataFarme对象的列顺序

### 参考资料：

CSDN: [Python dataframe修改列顺序｜Python](https://blog.csdn.net/a19990412/article/details/81945315)

### 1.修改DataFarme对象的列顺序

```python
# 将重新排好序的列标题做成列表
order = ['date', 'time', 'open', 'high', 'low', 'close', 'volumefrom', 'volumeto']

# 切片列置换
df = df[order]
```
