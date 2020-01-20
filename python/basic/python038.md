## 查看内存占用

一般只有在处理占用很大内存的数据时，用户才会关心镀锡i昂占用内存大小，典型的场景是读取大文件到内存。

### 1.查看对象占用内存大小

参考：[python查看对象占用的内存空间大小](https://blog.csdn.net/scene_2015/article/details/81779694)

python查看对象占用的内存空间大小，一共两步

（1）引用`sys`模块
（2）使用`sys.getsizeof`方法
举例如下：
```python
import sys
import pandas as pd

# 读取csv文件中的训练数据到 DataFrame 对象
train_transaction = pd.read_csv('../input/train_transaction.csv', index_col='TransactionID')

# 显示对象占用内存, 单位: 字节
print sys.getsizeof(train_transaction)
```

### 2.进程占用内存大小

参考：[python中使用psutil查看内存占用](https://blog.csdn.net/xiaodongxiexie/article/details/54633049)

需要用到`psutils`，模块，例子：

```python
import psutil
import os

info = psutil.virtual_memory()
print u'内存使用：',psutil.Process(os.getpid()).memory_info().rss
print u'总内存：',info.total
print u'内存占比：',info.percent
print u'cpu个数：',psutil.cpu_count()
```

其他内置的方法或属性还有：
```
boot_time
callable
collections
cpu_count
cpu_percent
cpu_stats
cpu_times
cpu_times_percent
disk_io_counters
disk_partitions
disk_usage
errno
functools
long
net_connections
net_if_addrs
net_if_stats
net_io_counters
os
pid_exists
pids
process_iter
pwd
signal
subprocess
swap_memory
sys
test
time
traceback
users
version_info
virtual_memory
wait_procs
win_service_get
win_service_iter
```