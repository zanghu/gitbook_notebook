## scipy: 稀疏矩阵存储法

参考资料：[GitHub: scipy几种稀疏矩阵表示](https://dirtysalt.github.io/html/types-of-scipy-sparse-matrix.html)

参考资料：[CSDN: scipy中稀疏矩阵coo_matrix, csr_matrix 的使用](https://blog.csdn.net/OOC_ZC/article/details/79605702)

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

csr_matrix矩阵里面有 `data`, `indptr`, `indices` 三个变量共同定位，其中row ith的数据是 `data[indptr[i]:indptr[i+1]]`, 对应的colum是 `indices[indptr[i]:indptr[i+1]`. 所以假设有N个row的话，那么indptr的大小是N+1. csc_matrix和csr_matrix相对应，只不过是通过column来定位.

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

### 4.不同格式稀疏矩阵相互转换的注意事项

这两种稀疏矩阵类型`csr_matrix`存储密度更大，但不易手工构建。`coo_matrix`存储密度相对小，但易于手工构建，常用方法为先手工构建`coo_matrix`，如果对内存要求高则使用 `tocsr()` 方法把coo_matrix转换为`csr_matrix`类型。

![](/assets/python036_01.png)

几种matrix之间相互转换的时间也是不对称的，从csc, csr -> coo转换的时间，相比coo->csc, csr的时间更短。所以如果只能使用一种中间存储格式的话，尽可能使用csc,csr而不是coo.