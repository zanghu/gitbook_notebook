## 查看内存占用

一般只有在处理占用很大内存的数据时，用户才会关心镀锡i昂占用内存大小，典型的场景是读取大文件到内存。

### 1.查看对象占用内存大小

python查看对象占用的内存空间大小，一共两步

（1）引用sys模块
（2）使用getsizeof()方法
举例如下：
```python
import sys
import pandas as pd

train_transaction = pd.read_csv('../input/train_transaction.csv', index_col='TransactionID')
 print sys.getsizeof(train_transaction)
 ```