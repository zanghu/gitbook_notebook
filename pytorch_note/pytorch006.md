## Pytorch: 难懂函数——torch.gather

参考资料：[http://www.cnblogs.com/darkknightzh/p/6836568.html](http://www.cnblogs.com/darkknightzh/p/6836568.html)

torch.gather函数的官方文档中解释比较复杂，而例子有关于简单

### 1.torch.gather的作用

其实从torch.gather子啊官方文档中的例子可以看出，起本质上是用于对torch.Tensor类对象独享进行元素置换的函数。

### 2.张量置换的分析

* 描述张量置换的充要条件

要描述一个张量置换，至少需要下面两个张量：

（1）原张量，即用来被置换的那个“模板”张量，置换后得到的张量的每个元素都来自于原张量；

（2）索引张量：即描述置换的映射关系的那个张量，每个元素是一个索引号，一般用来表示置换后张量中的一个元素对应于原张量中的那哪个元素；

* 简单的情况

首先考虑低维张量，以向量（一维张量）为例进行分析。

要描述对一个向量进行置换得到一个新的向量，所需的索引张量也是一个向量，例如：

原张量为: \[1, 2, 3\]，索引张量为\[2, 0, 1\]，那么置换后的张量为\[3, 1, 2\]

* 稍复杂的情况

接下来考虑原张量是矩阵（二维张量）的情况，如果希望将原矩阵通过元素置换办得到一个新矩阵，那么为了标明新矩阵每个元素在原矩阵的位置，必须为新矩阵每个位置上元素一个二元坐标，这个坐标就是新矩该位置元素在原矩阵中的行号和列号。


### 3.torch.gather参数说明

* 函数原型

`torch.gather(input, dim, index, out=None) → Tensor`

* 参数说明

input: torch.Tensor, 作为置换“基础”的原张量，被置换的一方

dim: 置换切面所垂直的轴向的索引号


```shell
CUDA_VISIBLE_DEVICES=1 python main.py
```

python代码中设定：

```python
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "2"
```

参考网址：[http://www.cnblogs.com/darkknightzh/p/6591923.html](http://www.cnblogs.com/darkknightzh/p/6591923.html)

### 2.使用函数 set\_device

```python
import torch
torch.cuda.set_device(id)
```

该函数见 pytorch-master\torch\cuda\__init\__.py。

不过官方建议使用CUDA\_VISIBLE\_DEVICES，不建议使用 set\_device 函数。

