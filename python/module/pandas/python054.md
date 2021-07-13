## Pandas：统计列取值的种类

### 参考资料：

知乎: [Pandas中的object数据类型｜Python](https://zhuanlan.zhihu.com/p/161073173)

pbpython: [Overview of Pandas Data Types](https://pbpython.com/pandas_dtypes.html)

### 1.统计单列数据

| Pandas dtype	| Python type	| NumPy type |	Usage |
| :---: | --- |--- |--- |
| object | str or mixed | string_, unicode_, mixed types | Text or mixed numeric and non-numeric values |
| int64	| int | int_, int8, int16, int32, int64, uint8, uint16, uint32, uint64 | Integer numbers |
| float64 | float | float_, float16, float32, float64 | Floating point numbers |
bool	bool	bool_	True/False values
datetime64	datetime	datetime64[ns]	Date and time values
timedelta[ns]	NA	NA	Differences between two datetimes
category	NA	NA	Finite list of text values

