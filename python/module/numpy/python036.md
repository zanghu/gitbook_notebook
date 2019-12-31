## scipy: 稀疏矩阵存储法

参考资料：[GitHub: scipy几种稀疏矩阵表示](https://dirtysalt.github.io/html/types-of-scipy-sparse-matrix.html)

参考资料：[CSDN: scipy中稀疏矩阵coo_matrix, csr_matrix 的使用](https://blog.csdn.net/OOC_ZC/article/details/79605702)

参考资料：[CSDN: csr_matrix和csc_matrix简析](https://blog.csdn.net/sinat_33741547/article/details/79878547)

### 1.三类稀疏矩阵

`scipy.sparse`里面有几种稀疏矩阵的表示：

* lil_matrix(row-based linked list matrix)
* dok_matrix(dictionary of keys matrix)
* coo_matrix(coordinate matrix)

`lil_matrix`实现上是基于row的链表实现，在创建阶段最好按照row的顺序插入或者是修改，适合增量构建。 
`dok_matrix`是认为坐标是(i,j)是key, 矩阵元素是value, 使用字典的方式进行存储，适合增量构建。 
`coo_matrix`有三个数组,row,col, data. 其中data[i] = matrix[row[i], col[i]]. 不太容易增量构建，适合批量构建。

### 2.coo_matrix的压缩矩阵

在`coo_matrix`的基础上进行压缩，根据压缩方式有几种矩阵表示：

`csr_matrix`: compressed sparse row matrix). 针对行做压缩
`csc_matrix`: compressed sparse column matrix). 针对列做压缩
`bsr_matrix`: 在csr_matrix基础上的优化，针对局部比较稠密的矩阵做压缩

关于这几种matrix的存储和运算优劣可以参考`scipy`文档

### 3.coo_matrix和csr_matrix存储格式分析

假设我们有个矩阵如下：

```python
array([[1., 2., 3., 0., 0.],
       [0., 4., 5., 0., 0.],
       [0., 0., 0., 0., 0.],
       [0., 0., 0., 0., 0.],
       [0., 0., 0., 0., 0.]])
```

```python
>>> m2 = scipy.sparse.coo_matrix(m.toarray())
>>> m3 = m2.tocsr()
>>> m4 = m2.tocsc()
```

（1）`coo_matrix`的存储结构

```python
>>> m2.data
Out[114]: array([1., 2., 3., 4., 5.])
```

```python
>>> m2.row
Out[115]: array([0, 0, 0, 1, 1], dtype=int32)
```

```python
>>> m2.col
Out[116]: array([0, 1, 2, 1, 2], dtype=int32)
```

（2）`csr_matrix`的存储结构

```python
>>> m3.data
Out[117]: array([1., 2., 3., 4., 5.])
```

```python
>>> m3.indptr
Out[118]: array([0, 3, 5, 5, 5, 5], dtype=int32)
```

```python
>>> m3.indices
Out[119]: array([0, 1, 2, 1, 2], dtype=int32)
```

（3）`csc_matrix`的存储结构

```python
>>> m4.data
Out[120]: array([1., 2., 4., 3., 5.])
```

```python
>>> m4.indptr
Out[121]: array([0, 1, 3, 5, 5, 5], dtype=int32)
```

```python
>>> m4.indices
Out[122]: array([0, 0, 1, 0, 1], dtype=int32)
```

### 4.csr_matrix/csc_matrix详细说明

以`csr_matrix`为例，请看下面的示例：

```python
>>> indptr = np.array([0, 2, 3, 6])
>>> indices = np.array([0, 2, 2, 0, 1, 2])
>>> data = np.array([1, 2, 3, 4, 5, 6])
>>> x = csr_matrix((data, indices, indptr), shape=(3, 3)).toarray()
```

**解释：**
（1）data表示数据，为[1, 2, 3, 4, 5, 6]
（2）shape表示矩阵的形状
（3）indices表示对应data中的数据，在压缩后矩阵中各行的下标，如：数据1在某行的0位置处，数据2在某行的2位置处，数据6在某行的2位置处。
（4）indptr表示压缩后矩阵中每一行所拥有数据的个数，如：[0 2 3 6]表示从第0行开始数据的个数，0表示默认起始点，0之后有几个数字就表示有几行，第一个数字2表示第一行有2 - 0 = 2个数字，因而数字1，2都第0行，第二行有3 - 2 = 1个数字，因而数字3在第1行，以此类推。

总之：
（1）data是矩阵按行压缩到一维向量再去掉所有非0元后的表示
（2）indices给出了data中每个元素在原始矩阵中的行号
（3）indptr给出了原始矩阵中每行的元素数，技术某一行元素数为0也不会省略
（4）shape[1]给出的原始矩阵列数对于`csr_matrix`是必要的，因为indptr只能给出原始矩阵行数；同样的道理，shape[0]对于`csc_matrix`是必要的。

稀疏矩阵x:

```python
>>> x
array([[1, 0, 2],
       [0, 0, 3],
       [4, 5, 6]])
```

这里介绍一个专门用来求向量中相邻元素的差值的函数：`numpy.eddif1d`

### 5.不同格式稀疏矩阵相互转换的注意事项

这两种稀疏矩阵类型`csr_matrix`存储密度更大，但不易手工构建。`coo_matrix`存储密度相对小，但易于手工构建，常用方法为先手工构建`coo_matrix`，如果对内存要求高则使用 `tocsr()` 方法把coo_matrix转换为`csr_matrix`类型。

![](/assets/python036_01.png)

几种matrix之间相互转换的时间也是不对称的，从csc, csr -> coo转换的时间，相比coo->csc, csr的时间更短。所以如果只能使用一种中间存储格式的话，尽可能使用csc,csr而不是coo.